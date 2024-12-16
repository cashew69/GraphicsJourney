//																								Shree
// header files
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


// global variable declarations
int bFullScreen = 0;

// Entry-point function
int main(int argc, char* argv[])
{
	// local variable declarations
	int width = 800;
	int height = 600;
	// local function declarations
	int initialize(void);
	void resize(int, int);
	void display(void);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void uninitialize(void);
	void setupRC(void);
	int drawline(int, int, int, int, int, int, int, int);

	// code
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("lightning=RTR018");

	// Set up orthographic projection with custom coordinates
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluOrtho2D(0.0, width, 0.0, height); // Set the coordinate range

	initialize();

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);
	setupRC();

	glutMainLoop();

	return(0);
}


int initialize(void)
{
	// code
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return(0);
}

void resize(int width, int height)
{
	// code
	glViewport(0, 0, width, height);
}

void setupRC(void)
{
	glOrtho (-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
}
int drawline(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	// code
	glBegin(GL_LINE_STRIP);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glVertex2f(x3, y3);
	glVertex2f(x4, y4);
	glEnd();
	return 0;

}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	int x1 = rand() % 30 - 15;
	int y1 = rand() % 20 + 70;
	int x2 = rand() % 20 - 10 + x1;
	int y2 = y1 - (rand() % 20 + 15);
	int x3 = rand() % 20 - 10 + x2;
	int y3 = y2 - (rand() % 20 + 15);
	int x4 = rand() % 20 - 10 + x3;
	int y4 = y3 - (rand() % 20 + 15);

	glColor3f(1.0f, 1.0f, 1.0f);
	drawline(x1, y1, x2, y2, x3, y3, x4, y4);
	glutPostRedisplay();
	glutSwapBuffers();


}

void keyboard(unsigned char key, int x, int y)
{
	// code
	switch (key)
	{
		case 27:
			glutLeaveMainLoop();
			break;
		case 'F':
		case 'f':
			if (bFullScreen == 0)
			{
				glutFullScreen();
				bFullScreen = 1;
			}
			else
			{
				glutLeaveFullScreen();
				bFullScreen = 0;
			}
			break;
		default:
			break;
	}
}

void mouse(int button, int state, int x, int y)
{
	// code
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			glutLeaveMainLoop();
			break;
		default:
			break;
	}
}

void uninitialize(void)
{
	// code
}

