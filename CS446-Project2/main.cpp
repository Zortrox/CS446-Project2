/* Matthew Clark
   CS 446 - Computer Graphics
================================================
	- WASD for camera movement
	- mouse move for camera looking
	- Q/E to change field of view
	- Esc to quit
*/

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#define PI 3.14159

enum keys {KEY_W, KEY_A, KEY_S, KEY_D, KEY_MAX};
bool keyPressed[KEY_MAX] = { false };

//screen width and height to modify matrices (like when resizing)
int screenWidth = 800;
int screenHeight = 600;
int mouseX = screenWidth / 2;
int mouseY = screenHeight / 2;

class Camera {
public:
	void look() {
		//set perspective
		GLfloat aspect = (GLfloat)screenWidth / (screenHeight > 0 ? screenHeight : 1);
		gluPerspective(m_fov, aspect, 0.1f, 1000.0f);

		//set lookAt
		GLfloat dx = cos(m_pitch) * sin(m_yaw);
		GLfloat dy = sin(m_pitch);
		GLfloat dz = cos(m_pitch) * cos(m_yaw);

		//move the camera
		if (keyPressed[KEY_W]) {
			m_x += dx * m_speedMod;
			m_y += dy * m_speedMod;
			m_z += dz * m_speedMod;
		}
		if (keyPressed[KEY_S]) {
			m_x -= dx * m_speedMod;
			m_y -= dy * m_speedMod;
			m_z -= dz * m_speedMod;
		}
		if (keyPressed[KEY_A]) {
			m_x += cos(m_yaw) * m_speedMod;
			m_z += -sin(m_yaw) * m_speedMod;
		}
		if (keyPressed[KEY_D]) {
			m_x -= cos(m_yaw) * m_speedMod;
			m_z -= -sin(m_yaw) * m_speedMod;
		}

		gluLookAt(m_x, m_y, m_z, m_x + dx, m_y + dy, m_z + dz, 0, 1, 0);
	}

	void changePitch(GLfloat pitch) {
		m_pitch += pitch * m_lookMod;

		//set between -PI/2 and PI/2
		if (m_pitch > PI / 2) m_pitch = (GLfloat)PI / 2;
		if (m_pitch < -PI / 2) m_pitch = (GLfloat)-PI / 2;
	}

	void changeYaw(GLfloat yaw) {
		m_yaw += yaw * m_lookMod;

		//normalize between 0 & 2*PI
		while (m_yaw > 2 * PI) {
			m_yaw -= 2 * (GLfloat)PI;
		}
		while (m_yaw < 0) {
			m_yaw += 2 * (GLfloat)PI;
		}
	}

	void changeFoV(GLfloat fov) {
		m_fov += fov;

		if (m_fov > 360) m_fov = 360.0f;
		else if (m_fov < 0) m_fov = 0.0f;
	}

private:
	GLfloat m_x = 0;
	GLfloat m_y = 0;
	GLfloat m_z = 0;
	GLfloat m_pitch = 0;
	GLfloat m_yaw = (GLfloat)-PI;

	GLfloat m_fov = 45.0f;

	GLfloat m_speedMod = 0.25;
	GLfloat m_lookMod = 0.5;
};

//shape to be transformed
class GLObject {
public:
	std::vector<GLfloat> buf_vertices;
	std::vector<GLfloat> buf_normals;
	std::vector<GLuint> buf_faces;
	std::vector<GLfloat> buf_colors;
};

Camera cam = Camera();
GLObject objModel;
bool mouseWarped = false;

void displayLoop(void) {
	//clear screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	//draw model
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	cam.look();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, -4, -10);

	//cool model
	GLfloat color[] = { 0.51f, 0.28f, 0.22f, 1.0f };	//brown
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &objModel.buf_vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &objModel.buf_normals[0]);
	glDrawElements(GL_QUADS, (GLsizei)objModel.buf_faces.size(), GL_UNSIGNED_INT, &objModel.buf_faces[0]);

	//draw GUI
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, screenHeight, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	//draw stuff on screen
	/*
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 200, 0);
	glVertex3f(100, 100, 0);
	glVertex3f(200, 200, 0);
	glEnd();
	*/

	//swap buffers (redraw screen)
	glutSwapBuffers();
}

//sets the screen's stored size & new viewport when the window is resized
void resizeScreen(int width, int height) {
	screenWidth = width;
	screenHeight = height;
	glViewport(0, 0, screenWidth, screenHeight);
}

void pressNormalKey(unsigned char key, int x, int y) {
	switch (key) {
	//quit when Esc key is pressed
	case 27:
		exit(1);
		break;
	case 'w':
		keyPressed[KEY_W] = true;
		break;
	case 'a':
		keyPressed[KEY_A] = true;
		break;
	case 's':
		keyPressed[KEY_S] = true;
		break;
	case 'd':
		keyPressed[KEY_D] = true;
		break;
	case 'q':
		cam.changeFoV(-10);
		break;
	case 'e':
		cam.changeFoV(10);
		break;
	}
}

//when a key is released (not needed)
void releaseNormalKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		keyPressed[KEY_W] = false;
		break;
	case 'a':
		keyPressed[KEY_A] = false;
		break;
	case 's':
		keyPressed[KEY_S] = false;
		break;
	case 'd':
		keyPressed[KEY_D] = false;
		break;
	}
}

//handler for mouse buttons
void mouseButton(int button, int state, int x, int y) {
	switch (button) {
	//handle the left mouse button
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {
			
		} else {// state = GLUT_DOWN
			
		}
		break;
	//handle the right mouse button
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP) {

		} else {// state = GLUT_DOWN
			
		}
		break;
	//handle the middle mouse button
	case GLUT_MIDDLE_BUTTON:
		if (state == GLUT_UP) {

		}
		else {// state = GLUT_DOWN
			
		}
		break;
	}
}

void mouseMove(int x, int y) {
	int oldMouseX = mouseX;
	int oldMouseY = mouseY;
	mouseX = x;
	mouseY = y;

	GLfloat deltaPitch = (GLfloat)(PI * (oldMouseY - mouseY) / 180);
	GLfloat deltaYaw = (GLfloat)(PI * (oldMouseX - mouseX) / 180);

	cam.changePitch(deltaPitch);
	cam.changeYaw(deltaYaw);
}

void init() {
	objModel = GLObject();

	std::ifstream file("tower-normals-notri.txt");
	if (file.is_open()) {
		std::string line;

		std::vector<GLfloat> tempVertices;
		std::vector<GLfloat> tempNormals;

		bool firstFace = true;

		while(std::getline(file, line))
		{
			std::stringstream ss(line);

			std::string type;
			ss >> type;

			if (type == "v") {
				GLfloat v1, v2, v3;
				ss >> v1;
				ss >> v2;
				ss >> v3;

				objModel.buf_vertices.push_back(v1);
				objModel.buf_vertices.push_back(v2);
				objModel.buf_vertices.push_back(v3);
			}
			else if (type == "vn") {
				GLfloat n1, n2, n3;
				ss >> n1;
				ss >> n2;
				ss >> n3;

				tempNormals.push_back(n1);
				tempNormals.push_back(n2);
				tempNormals.push_back(n3);
			}
			else if (type == "f") {
				if (firstFace) {
					objModel.buf_normals.resize(objModel.buf_vertices.size());
				}

				GLuint v1, n1, v2, n2, v3, n3, v4, n4;
				ss >> v1;
				ss >> n1;
				ss >> v2;
				ss >> n2;
				ss >> v3;
				ss >> n3;
				ss >> v4;
				ss >> n4;

				//faces
				objModel.buf_faces.push_back(v1 - 1);
				objModel.buf_faces.push_back(v2 - 1);
				objModel.buf_faces.push_back(v3 - 1);
				objModel.buf_faces.push_back(v4 - 1);
				//normal buffer
				objModel.buf_normals[v1 - 1] = tempNormals[n1 - 1];
				objModel.buf_normals[v2 - 1] = tempNormals[n2 - 1];
				objModel.buf_normals[v3 - 1] = tempNormals[n3 - 1];
				objModel.buf_normals[v4 - 1] = tempNormals[n4 - 1];
			}
		}

		file.close();
	}
}

//redraw the screen @ 60 FPS
void redraw(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, redraw, 0);
}

int main(int argc, char* argv[]) {
	//create double-buffer display & setup
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glEnable(GL_MULTISAMPLE);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 50);
	glViewport(0, 0, screenWidth, screenHeight);
	glutCreateWindow("CS446 - Project 1");

	//handle drawing and screen resizing
	glutDisplayFunc(displayLoop);
	glutReshapeFunc(resizeScreen);
	//start screen refresh timer
	glutTimerFunc(1000 / 60, redraw, 0);

	//ignore repeated key holding
	glutIgnoreKeyRepeat(true);

	//handle keyboard
	//glutSpecialFunc(pressKey);
	//glutSpecialUpFunc(releaseKey);
	glutKeyboardFunc(pressNormalKey);
	glutKeyboardUpFunc(releaseNormalKey);

	//handle mouse
	//glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(screenWidth / 2, screenHeight / 2);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	//initialize the 3D model
	init();

	//light
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	GLfloat lightPos[] = { 0, 20, -10, 0 };
	GLfloat lightAmb[] = { .2f, .2f, .2f, 1 };
	GLfloat lightDif[] = { 1, 1, 1, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);

	//start the main OpenGL loop
	glutMainLoop();
	return 0;
}