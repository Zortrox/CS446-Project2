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
#define CIRCLE_RES 20	//circle clicked resolution (how many segments around circle)
#define CIRCLE_RAD 20	//circle clicked radius

//circles to draw 
class Circle {
public:
	Circle(int x, int y, int r) {
		m_x = x; 
		m_y = y;
		m_r = r;
	}

	int m_x = 0;	//x position
	int m_y = 0;	//y position
	int m_r = 0;	//radius
};

//shape to be transformed
class GLObject {
public:
	double hScale = 1.0;
	double vScale = 1.0;
	double xPos = 400;
	double yPos = 300;
	int angle = 0;
};

//vector of clicked circle objects
std::vector<Circle*> vecCircles;
//transformed shape (cat head)
GLObject objShape;
//if menu is open
bool menuOpen = false;

//screen width and height to modify matrices (like when resizing)
int screenWidth = 800;
int screenHeight = 600;

//draw a circle from the object
void drawCircle(Circle* cir, int reso) {
	glBegin(GL_POLYGON);
	for (int j = 0; j < reso; j++) {
		double xPos = cir->m_x + cir->m_r * cos((2 * PI) / reso * j);
		double yPos = cir->m_y + cir->m_r * sin((2 * PI) / reso * j);

		glVertex2f((float)xPos, (float)yPos);
	}
	glEnd();
}

//draw a circle from variables
void drawCircle(int x, int y, int r, int resolution) {
	Circle temp(x, y, r);
	drawCircle(&temp, resolution);
}

void displayLoop(void) {
	//clear screen to black
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//load base draw matrix
	glLoadIdentity();
	//change to use screen pixels as coordinate system.
	gluOrtho2D(0, screenWidth, screenHeight, 0);

	//draw clicked circles 
	glColor3f(1.0f, 1.0f, 1.0f);
	for (size_t i = 0; i < vecCircles.size(); i++) {
		drawCircle(vecCircles.at(i), CIRCLE_RES);
	}

	//transform draw matrix
	glTranslated(objShape.xPos, objShape.yPos, 0);
	glRotated(objShape.angle, 0, 0, 1);
	glScaled(objShape.hScale, objShape.vScale, 1);
	
	//draw cat head (tan circle)
	glColor3f(0.82f, 0.71f, 0.55f);
	drawCircle(0, 0, 40, 40);

	//draw cat ears (tan triangles)
	glBegin(GL_TRIANGLES);
	glVertex2f(-35, -10);
	glVertex2f(-50, -60);
	glVertex2f(-15, -35);
	glVertex2f(35, -10);
	glVertex2f(50, -60);
	glVertex2f(15, -35);
	glEnd();

	//draw cat eyes (green circles)
	glColor3f(0.24f, 0.70f, 0.44f);
	drawCircle(-15, -15, 7, 10);
	drawCircle(15, -15, 7, 10);

	//draw cat irises (black triangles)
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	//left iris
	glVertex2f(-16, -15);
	glVertex2f(-14, -15);
	glVertex2f(-15, -23);
	
	glVertex2f(-16, -15);
	glVertex2f(-14, -15);
	glVertex2f(-15, -7);

	//right iris
	glVertex2f(16, -15);
	glVertex2f(14, -15);
	glVertex2f(15, -23);

	glVertex2f(16, -15);
	glVertex2f(14, -15);
	glVertex2f(15, -7);
	glEnd();

	//draw cat nose (pink triangle)
	glColor3f(.80f, 0.51f, 0.55f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, -2);
	glVertex2f(-6, 9);
	glVertex2f(6, 9);
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

//get whether menu is opened or not (stops creating circles)
void processMenuStatus(int status, int x, int y) {
	if (status == GLUT_MENU_IN_USE) menuOpen = true;
	else menuOpen = false;
}

//determine function by menu item click
void menu(int item) {
	switch (item) {
	case 1:
		objShape.xPos -= 5;
		break;
	case 2:
		objShape.xPos += 5;
		break;
	case 3:
		objShape.yPos -= 5;
		break;
	case 4:
		objShape.yPos += 5;
		break;
	case 5:
		objShape.hScale *= 2;
		break;
	case 6:
		objShape.hScale *= 0.5;
		break;
	case 7:
		objShape.vScale *= 2;
		break;
	case 8:
		objShape.vScale *= 0.5;
		break;
	case 9:
		objShape.angle += 10;
		break;
	case 10:
		objShape.angle -= 10;
		break;
	default:
		std::cout << "menu number " << item << " pressed" << std::endl;
		break;
	}
}

//create the popup menu that shows on right click
void createMenu() {
	int subTranslate = glutCreateMenu(menu);
	glutAddMenuEntry("Left", 1);
	glutAddMenuEntry("Right", 2);
	glutAddMenuEntry("Up", 3);
	glutAddMenuEntry("Down", 4);

	int subScale = glutCreateMenu(menu);
	glutAddMenuEntry("H-Enlarge", 5);
	glutAddMenuEntry("H-Shrink", 6);
	glutAddMenuEntry("V-Enlarge", 7);
	glutAddMenuEntry("V-Shrink", 8);

	int subRotate = glutCreateMenu(menu);
	glutAddMenuEntry("Clockwise", 9);
	glutAddMenuEntry("Counter-Clockwise", 10);

	int mainMenu = glutCreateMenu(menu);
	glutAddSubMenu("Translate", subTranslate);
	glutAddSubMenu("Scale", subScale);
	glutAddSubMenu("Rotate", subRotate);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//special key presses (not needed in this program)
/*
void pressKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:

		break;
	case GLUT_KEY_DOWN:
		
		break;
	}
}

void releaseKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		
		break;
	case GLUT_KEY_DOWN:

		break;
	}
}*/

void pressNormalKey(unsigned char key, int x, int y) {
	switch (key) {
	//quit when Q key is pressed
	case 'Q':
	case 'q':
		exit(1);
		break;
	}
}

//when a key is released (not needed)
void releaseNormalKey(unsigned char key, int x, int y) {

}

//handler for mouse buttons
void mouseButton(int button, int state, int x, int y) {
	switch (button) {
	//handle the left mouse button
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {
			
		} else {// state = GLUT_DOWN
			//draw a circle if the menu isn't open
			if (!menuOpen) {
				vecCircles.push_back(new Circle(x, y, CIRCLE_RAD));
			}
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
			objShape.xPos = x;
			objShape.yPos = y;
		}
		break;
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

	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 50);
	glViewport(0, 0, screenWidth, screenHeight);
	glutCreateWindow("CS446 - Project 1");
	
	//create menu and set menu handler
	createMenu();
	glutMenuStatusFunc(processMenuStatus);

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
	glutMouseFunc(mouseButton);
	//glutMotionFunc(mouseMove);

	//start the main OpenGL loop
	glutMainLoop();
	return 0;
}