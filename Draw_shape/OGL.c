//																								Shree
// header files
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159
#define MAX_LINES 100


// global variable declarations
int bFullScreen = 0;
int Width = 1000;
int Height = 1000;


float startX[MAX_LINES];
float startY[MAX_LINES];
float endX[MAX_LINES];
float endY[MAX_LINES];
float colors[MAX_LINES][3];
int lineCount = 0;

float StartX = 0.0;
float StartY = 0.0;
float EndX = 0.0;
float EndY = 0.0;

int isDrawing = 0;


// Entry-point function
int main(int argc, char* argv[])
{
    // local variable declarations

    // local function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void mouse(int, int, int, int);
    void uninitialize(void);
    void leftclick(float, float);
    void rightclick(float, float);
    void drawline(float, float, float, float, float, float ,float);
    void removeline(float, float, int);
    void mouse_motion(int, int);
    void printvertex();

    // code
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(Width, Height);
    glutInitWindowPosition(800, 0);
    glutCreateWindow("Earth Orbits Sun = Nikhil Sathe");

    initialize();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_motion);


    glutMainLoop();
    return(0);
}

int initialize(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return(0);
}



void drawline(float x1, float y1, float x2, float y2, float r, float g, float b) {

    glLineWidth(5.0f);
    glBegin(GL_LINES);

    glColor3f(r, g, b);
    glVertex2f(x1, y1); // Starting point
    glVertex2f(x2, y2); // Ending point
    glEnd();
}

void mouse_motion(int x, int y) {
    if (isDrawing) {
        EndX = (x * 2.0 / Width) - 1; // Normalized x
        EndY = 1 - (y * 2.0 / Height); // Normalized y
        glutPostRedisplay();
    }
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(1.0);




    // Draw grid lines on Y Axis
    for (float point = -1.0f; point <= 1.0; point += 0.05) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-1.0f, point);
        glVertex2f(1.0f, point);
        glEnd();
    }

    // Draw grid lines on X Axis
    for (float point = -1.0f; point <= 1.0; point += 0.05) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(point, -1.0f);
        glVertex2f(point, 1.0f);
        glEnd();
    }

    // Draw axes
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f); // Y Axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f); // X Axis
    glEnd();

    // previously stored lines
    for (int i = 0; i < lineCount; i++) {
        drawline(startX[i], startY[i], endX[i], endY[i], colors[i][0], colors[i][1], colors[i][2]);
    }

    // Draw
    if (isDrawing) {
        drawline(StartX, StartY, EndX, EndY, 1.0f, 1.0f, 0.0f); // Yellow while dragging
    } else if (lineCount > 0) {
        drawline(StartX, StartY, EndX, EndY, 1.0f, 0.0f, 0.0f); // Red when finished
    }

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:
            glutLeaveMainLoop();
            break;
        case 'F':
        case 'f':
            if (bFullScreen == 0) {
                glutFullScreen();
                bFullScreen = 1;
            } else {
                glutLeaveFullScreen();
                bFullScreen = 0;
            }
            break;
        default:
            break;
    }
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                if (lineCount == 0 || !isDrawing) {
                    StartX = (x * 2.0 / Width) - 1;
                    StartY = 1 - (y * 2.0 / Height);
                }
                isDrawing = 1;

            } else if (state == GLUT_UP) {
                EndX = (x * 2.0 / Width) - 1;
                EndY = 1 - (y * 2.0 / Height);

                if (lineCount < MAX_LINES) {
                    startX[lineCount] = StartX;
                    startY[lineCount] = StartY;
                    endX[lineCount] = EndX;
                    endY[lineCount] = EndY;
                    colors[lineCount][0] = 1.0f;
                    colors[lineCount][1] = 0.0f;
                    colors[lineCount][2] = 0.0f;

                    printf("glVertex2f(%f, %f); // start\n", StartX, StartY);
                    printf("glVertex2f(%f, %f); // end\n", EndX, EndY);

                    lineCount++;
                }

                isDrawing = 0;


            }

        glutPostRedisplay();
        }
    }
}

void uninitialize(void) {
    // code
}
