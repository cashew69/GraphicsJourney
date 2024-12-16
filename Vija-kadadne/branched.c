//																								Shree
// header files
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
    void drawlines(float x, float y, float angle, float length, int depth);


    // code
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("branched lightning RTR018");

    initialize();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutCloseFunc(uninitialize);
    setupRC();

    glutMainLoop();

    return 0;
}

int initialize(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    srand(time(0));
    return 0;
}

void resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void setupRC(void)
{
    glOrtho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
}

void drawlines(float x, float y, float angle, float length, int depth) {
    if (depth == 0) return;

    float xEnd = x + length * cos(angle);
    float yEnd = y + length * sin(angle);

    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(xEnd, yEnd);
    glEnd();

    if (rand() % 3 == 0) {
        drawlines(xEnd, yEnd, angle + (rand() % 30 + 10) * 3.14 / 180, length * 0.7, depth - 1);
    }
    if (rand() % 3 == 0) {
        drawlines(xEnd, yEnd, angle - (rand() % 30 + 10) * 3.14 / 180, length * 0.7, depth - 1);
    }
    drawlines(xEnd, yEnd, angle + (rand() % 10 - 5) * 3.14 / 180, length * 0.8, depth - 1);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f);

    float startX = rand() % 20 - 10;
    float startY = 80.0f;
    float angle = -3.14f / 2;
    float length = 10.0f;
    int maxDepth = 6;

    drawlines(startX, startY, angle, length, maxDepth);

    glutSwapBuffers();
    glutPostRedisplay();

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

