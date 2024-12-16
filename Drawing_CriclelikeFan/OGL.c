//																								Shree
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
    glutCreateWindow("Earth Orbits Sun =NikhilSathe");
    glutTimerFunc(0, animate, 0); // Starts the animation loop


    initialize();

    //glutReshapeFunc(resize);
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

float points[1000] = {};
int count = 0;

void drawcircle(float x, float y, float radius, float r, float g, float b, float ObjAngle)
{
    float ObjX = 0.0f;
    float ObjY = 0.0f;
    //float ObjAngle = 0.0f;

    // Circle
    glBegin(GL_LINES);
    /*for (ObjAngle = (0.00f); ObjAngle <= (2.0f*PI); ObjAngle += 0.01f)
    //{*/

        for (int i = 0 ; i <= count; i++)
        {
            ObjAngle = points[i];
            ObjX = x + radius * sin(ObjAngle);
            ObjY = y + radius * cos(ObjAngle);



            glColor3f(r,g,b);

            glVertex2f(x + radius * sin(0.0f), y + radius * cos(0.0f));
            glVertex2f(ObjX, ObjY);

            glColor3f(1.0f,0.0f,0.0f);


            glVertex2f(x, y);
            glVertex2f(ObjX, ObjY);


        }

    //}

    glEnd();

}



void display(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT);



/*
    glLineWidth(1.0f);
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
    }*/

//for (float ObjAngle = (0.00f); ObjAngle <= (2.0f*PI); ObjAngle += 0.01f)
//{
    glLineWidth(1.0f);
    drawcircle(0.0f, 0.0f, 0.35f, 0.2078f,0.5686f,0.7529f, angle);
    points[count] = angle;
//}


    glutSwapBuffers();

}

void animate(int value) {
    angle += 0.005f;
    count += 1;
    if (angle > 365.256f) {
        angle -= 365.256f;
        count = 0 ;
    }

    glutPostRedisplay(); // Requests redraw
    glutTimerFunc(100, animate, 0); // Calls in 10ms

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
