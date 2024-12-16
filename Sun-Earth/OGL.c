// Jai Shree Ram!!!
// header files
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#define PI 3.1415


// global variable declarations
int bFullScreen = 0;
float position = 0.0f;
float angle = 0.0f; // Initial angle
const float radius = 0.9f; // Radius of the circular path
const float centerX = 0.0f; // X coordinate of circle center
const float centerY = 0.0f; // Y coordinate of circle center
float EARTH_X = 0.0f;
float EARTH_Y = 0.0f;

// Entry-point function
int main(int argc, char* argv[])
{
    // local variable declarations
    int width = 1080;
    int height = 1080;
    // local function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void uninitialize(void);
    void animate(int);
    void EarthOrbit();
    void earth(void);
    void sun(void);
    void moon(void);
    // code
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Earth Orbits Sun = NikhilSathe");
    glutTimerFunc(0, animate, 0);


    initialize();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutCloseFunc(uninitialize);

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

void earth(void)
{
    float ObjX = 0.0f;
    float ObjY = 0.0f;
    float ObjAngle = 0.0f;

    glBegin(GL_POLYGON);
    for (ObjAngle = (0.00f); ObjAngle <= (2.0f*PI); ObjAngle += 0.01f)
    {
        ObjX = 0.03f * sin(ObjAngle);
        ObjY = 0.03f * cos(ObjAngle);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex2f(ObjX, ObjY);
    }
    glEnd();

}

void moon(void)
{
    float ObjX = 0.0f;
    float ObjY = 0.0f;
    float ObjAngle = 0.0f;

    glBegin(GL_POLYGON);
    for (ObjAngle = 0.0f; ObjAngle <= (2.0f * PI); ObjAngle += 0.01f)
    {
        ObjX = 0.0029357798f * sin(ObjAngle);
        ObjY = 0.0029357798f * cos(ObjAngle);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(ObjX, ObjY);
    }
    glEnd();

}

void sun(void)
{
    float ObjX = 0.0f;
    float ObjY = 0.0f;
    float ObjAngle = 0.0f;

    glBegin(GL_POLYGON);
    for (ObjAngle = 0.0f-(PI/6.0f); ObjAngle <= (2.0f * PI); ObjAngle += 0.01f)
    {
        ObjX = 0.25f * sin(ObjAngle);
        ObjY = 0.25f * cos(ObjAngle);
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f(ObjX, ObjY);
    }
    glEnd();

}


void EarthOrbit()
{
    glLoadIdentity();
    float x = centerX + radius * cos(angle);
    float y = centerY + radius * sin(angle);

    EARTH_X = x;
    EARTH_Y = y;

    glTranslatef(x, y, 0.0f);

    earth();

}

void MoonOrbit()
{
    glLoadIdentity();
    float x = EARTH_X + 0.05 * cos(angle*10);
    float y = EARTH_Y + 0.05 * sin(angle*10);

    glTranslatef(x, y, 0.0f);

    moon();

}
void display(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    EarthOrbit();
    glPopMatrix();

    glPushMatrix();
    MoonOrbit();
    glPopMatrix();


    // Y Axis
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(0.0f,1.0f);
    glVertex2f(0.0f,-1.0f);
    glEnd();

    // x Axis
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(1.0f,0.0f);
    glVertex2f(-1.0f,0.0f);
    glEnd();

    //lines on Y Axis
    for (float point = -1.0f; point <= 1.0 ; point += 0.05 ) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.01f,point);
        glVertex2f(0.01f,point);
        glEnd();
    }

    //lines on X Axis
    for (float point = -1.0f; point <= 1.0 ; point += 0.05 ) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(point, -0.01f);
        glVertex2f(point, 0.01f);
        glEnd();
    }

    // Drawing sun on top of Axislines
    //sun();
    glPushMatrix();
    float x = (0.25f+(sqrt(3.0f)/2.0f))/10.0f + 0.01f ;
    glTranslatef(x,0.0f, 0.0f);
    sun();
    glPopMatrix();
    glutSwapBuffers();

}

void animate(int value) {
    angle += 0.005f;
    if (angle > 365.256f) {
        angle -= 365.256f;
    }

    glutPostRedisplay(); // Requests redraw
    glutTimerFunc(10, animate, 0); // Calls in 10ms

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


void uninitialize(void)
{
    // code
}
