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
float movx = -1.0f;
float movy = 0.0f;
float scale = 0.1f;
int UFO_light_color = 0;


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
    void drawcircle(float, float, float, float, float , float, float, float, int);
    void ufolight(float, float, float, float, int, float);
    void ufo(float, float, int, float);
    void ufolightschain(float , float , float, float );
    void flickeringUFOlights(int);

    // code
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Earth Orbits Sun =NikhilSathe");
    glutTimerFunc(0, animate, 0); // Starts the animation loop
    glutTimerFunc(0, flickeringUFOlights, 0); // Starts the animation loop


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

void drawcircle(float x, float y, float radius_vert, float radius_hor, float r, float g, float b, float alpha, int fill)
{
    float ObjX = 0.0f;
    float ObjY = 0.0f;
    float ObjAngle = 0.0f;

    // Circle
    if (fill == 1){
        glBegin(GL_POLYGON);
    }
    else {
        glBegin(GL_POINTS);
    }

    for (ObjAngle = (0.00f); ObjAngle <= (2.0f*PI); ObjAngle += 0.01f)

    {
        ObjX = x + radius_hor * sin(ObjAngle);
        ObjY = y + radius_vert * cos(ObjAngle);

        glColor4f(r,g,b, alpha);

        glVertex2f(ObjX, ObjY);
    }


    glEnd();

}

void ufolight(float originx, float originy, float landheight, float landwidth, int isscaling, float scaleby)
{
    glColor4f(1.0f, 0.9f, 0.5f, 0.0001f);
    float initialstrech = 0.1f;
    if (isscaling == 1){
        //originx = originx * scaleby;
        //originy = originy * scaleby;
        landwidth = landwidth * scaleby;
        landheight = landheight * scaleby;
        initialstrech = initialstrech * scaleby;
    }


   glBegin(GL_QUADS);
        glVertex2f(originx-initialstrech, originy-initialstrech);
        glVertex2f(originx+initialstrech, originy-initialstrech);

        glVertex2f(originx+initialstrech+landwidth, originy-landheight);
        glVertex2f(originx-initialstrech-landwidth, originy-landheight);

    glEnd();


    drawcircle(originx ,originy+(landheight*-1), (landwidth/4.0f),landwidth+initialstrech, 1.0f, 0.9f, 0.5f, 0.02f, 1 );



}
/*void ufolightschain(float originx, float y_intercept, float till, float scaleby)
{
    //y = 9*x*x
    float y = 0.0f;
    for(float x = originx; x <= till  ; x +=0.01f )
    {
        y = ((0.3*((x)*(x)))+ y_intercept);
        drawcircle(x , y , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.9f, 0.5f, 0.02f, 1);
        drawcircle(-x , -y , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.9f, 0.5f, 0.02f, 1);
    }
}*/

void ufo(float originx, float originy, int isscaling, float scaleby)
{

    float rad_cockpit = 0.15;
    float rad_body_vert = 0.12;
    float rad_body_horz = 0.39;

    if (isscaling == 1){


        //originx = originx * scaleby;
        //originy = originy * scaleby;
        rad_cockpit = rad_cockpit * scaleby;
        rad_body_vert = rad_body_vert * scaleby;
        rad_body_horz = rad_body_horz * scaleby;

    }





/*
  void drawcircle(float x, float y, float radius_vert, float radius_hor, float r, float g, float b, int fill)
*/
    // SpaceShip
    drawcircle(originx, originy, rad_cockpit, rad_cockpit, 0.3f, 0.5f, 0.5f, 0.02f, 1);
    drawcircle(originx, originy-(0.1*scaleby), rad_body_vert, rad_body_horz, 0.5f, 0.9f, 0.5f, 0.01f, 1);


    // center-light
    drawcircle(originx , originy-(0.15*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.9f, 0.5f, 0.02f, 1);

    if (UFO_light_color == 0)
    {
        // most left
        drawcircle(originx-(0.35*scaleby) , originy-(0.09*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 0.0f, 0.671f, 0.059f, 0.02f, 1);
        // most right
        drawcircle(originx+(0.35*scaleby) , originy-(0.09*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 0.0f, 0.671f, 0.059f, 0.02f, 1);
        // close left
        drawcircle(originx-(0.15*scaleby) , originy-(0.12*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.0f, 0.0f, 0.02f, 1);
        // close right
        drawcircle(originx+(0.15*scaleby) , originy-(0.12*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.0f, 0.0f, 0.02f, 1);
    }
    else
    {
        // most left
        drawcircle(originx-(0.35*scaleby) , originy-(0.09*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.0f, 0.0f, 0.02f, 1);
        // most right
        drawcircle(originx+(0.35*scaleby) , originy-(0.09*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 1.0f, 0.0f, 0.0f, 0.02f, 1);
        // close left
        drawcircle(originx-(0.15*scaleby) , originy-(0.12*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 0.0f, 0.671f, 0.059f, 0.02f, 1);
        // close right
        drawcircle(originx+(0.15*scaleby) , originy-(0.12*scaleby) , 0.015f*scaleby, 0.015f*scaleby, 0.0f, 0.671f, 0.059f, 0.02f, 1);
    }



}




void display(void)
{
    // code
    glClear(GL_COLOR_BUFFER_BIT);


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
    for (float point = -1.0f; point <= 1.0 ; point += 0.1 ) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(-0.01f,point);
        glVertex2f(0.01f,point);
        glEnd();
    }

    //lines on X Axis
    for (float point = -1.0f; point <= 1.0 ; point += 0.1 ) {
        glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2f(point, -0.01f);
        glVertex2f(point, 0.01f);
        glEnd();
    }





    glColor4f(1.0f, 0.9f, 0.5f, 0.0f);
    // UFLOLIGHT
    if (movx == 0.5f)
    {ufolight(movx, movy-(0.1*scale), 0.9, 0.4, 1, scale);}

    // SpaceShip
    ufo(movx, movy, 1, scale);
    //ufolightschain(movx, 0.0f-(0.1f), 0.39f, scale );


    //drawcircle(0.0f, 0.0f, 0.02f, 0.02f, 1.0f, 0.9f, 0.5f, 1);

    glutSwapBuffers();

}

void flickeringUFOlights(int value)
{
    if (UFO_light_color == 1)
    {
        UFO_light_color = 0;
    }
    else
    {
        UFO_light_color = 1;
    }
    glutTimerFunc(350, flickeringUFOlights, 0); // Calls in 10ms

}

void animate(int value)
{

    if (movx >= 0.5f)
    {

        movx = 0.5;
        movy = movy;
        scale = scale;

    }
    else{
        movx = movx + 0.001f;
        //movy = (0.3*((movx)*(movx)));
        movy = (sin(movx)*0.5);
        scale += 0.0005f;
    }





    //printf("CALLING UFO with movx: %f \n", movx);


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
