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
	std::vector<GLfloat> buf_colors;
};

Camera cam = Camera();
GLObject objModel;
bool mouseWarped = false;

void displayLoop(void) {
	//clear screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//draw model
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	cam.look();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -10);

	//cool colored cube
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &objModel.buf_vertices[0]);
	glColorPointer(3, GL_FLOAT, 0, &objModel.buf_colors[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

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

	
	objModel.buf_colors = {
		0.9129f, 0.2869f, 0.3804f,
		0.0905f, 0.8409f, 0.2749f,
		0.3311f, 0.4323f, 0.2830f,
		0.3952f, 0.3406f, 0.9958f,
		0.1566f, 0.7251f, 0.0676f,
		0.2462f, 0.5910f, 0.8968f,
		0.3476f, 0.5900f, 0.8106f,
		0.3273f, 0.9957f, 0.4397f,
		0.1233f, 0.8321f, 0.3267f,
		0.4228f, 0.7629f, 0.4454f,
		0.6090f, 0.6238f, 0.3802f,
		0.4040f, 0.9523f, 0.0277f,
		0.5113f, 0.6947f, 0.0112f,
		0.3262f, 0.4916f, 0.1642f,
		0.2375f, 0.6212f, 0.4669f,
		0.1308f, 0.1406f, 0.0793f,
		0.6079f, 0.2687f, 0.8032f,
		0.7420f, 0.3562f, 0.7075f,
		0.6217f, 0.0282f, 0.9930f,
		0.0482f, 0.9542f, 0.0747f,
		0.8987f, 0.7082f, 0.3587f,
		0.6970f, 0.6384f, 0.0869f,
		0.9515f, 0.7175f, 0.5247f,
		0.6862f, 0.0376f, 0.7484f,
		0.0726f, 0.4401f, 0.6465f,
		0.3212f, 0.0405f, 0.6477f,
		0.9670f, 0.4873f, 0.5832f,
		0.5030f, 0.2822f, 0.1609f,
		0.6579f, 0.8766f, 0.3217f,
		0.0131f, 0.9865f, 0.2878f,
		0.8901f, 0.2658f, 0.0933f,
		0.6414f, 0.4615f, 0.1820f,
		0.1086f, 0.5049f, 0.8790f,
		0.3964f, 0.6547f, 0.6725f,
		0.3330f, 0.0772f, 0.5582f,
		0.8698f, 0.4786f, 0.6829f
    };
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
	glutSetCursor(GLUT_CURSOR_NONE);
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