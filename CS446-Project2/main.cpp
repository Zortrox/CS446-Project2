/* Matthew Clark
   CS 446 - Computer Graphics
================================================
	- Left click for circles
	- Right click to transform shape (cat) with menu
	- Middle click to move shape (cat)
*/

#include <GL\glew.h>
#include <GL\freeglut.h>
#include <vector>
#include <iostream>

#define PI 3.14159

enum keys {KEY_W, KEY_A, KEY_S, KEY_D, KEY_MAX};
bool keyPressed[KEY_MAX] = { false };

class Camera {
public:
	void look() {
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
			m_x -= -dz * m_speedMod;
			m_z -=  dx * m_speedMod;
		}
		if (keyPressed[KEY_D]) {
			m_x += -dz * m_speedMod;
			m_z +=  dx * m_speedMod;
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

private:
	GLfloat m_x = 0;
	GLfloat m_y = 0;
	GLfloat m_z = 0;
	GLfloat m_pitch = 0;
	GLfloat m_yaw = (GLfloat)-PI;

	GLfloat m_speedMod = 0.25;
	GLfloat m_lookMod = 0.5;
};

//shape to be transformed
class GLObject {
public:
	std::vector<GLfloat> buf_vertices;
};

Camera cam = Camera();
GLObject objModel;

//screen width and height to modify matrices (like when resizing)
int screenWidth = 800;
int screenHeight = 600;
int mouseX = screenWidth / 2;
int mouseY = screenHeight / 2;

void displayLoop(void) {
	//clear screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//draw model
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLfloat aspect = (GLfloat)screenWidth / (screenHeight > 0 ? screenHeight : 1);
	gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
	cam.look();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -10);

	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
									  // Top face (y = 1.0f)
									  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glColor3f(1.0f, 0.0f, 0.0f);     // Red
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube

	glTranslatef(10, 0, 0);

	//cube #2
	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &objModel.buf_vertices[0]);
	//glColorPointer(3, GL_FLOAT, 0, colors);
	glDrawArrays(GL_TRIANGLES, 0, 18);
	glColor3f(0.0f, 0.0f, 1.0f);     // Blue
	glDrawArrays(GL_TRIANGLES, 18, 18);

	//draw GUI
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, screenHeight, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_TRIANGLES);
	glVertex3f(0, 200, 0);
	glVertex3f(100, 100, 0);
	glVertex3f(200, 200, 0);
	glEnd();

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
	objModel.buf_vertices = {
		-1.0f,-1.0f,-1.0f, // triangle 1 : begin
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f,-1.0f, // triangle 2 : begin
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triangle 2 : end
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	/*
	GLfloat colors[] = {
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0
    };
	*/
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
	glutWarpPointer(screenWidth / 2, screenHeight / 2);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);

	//initialize the 3D model
	init();

	//start the main OpenGL loop
	glutMainLoop();
	return 0;
}