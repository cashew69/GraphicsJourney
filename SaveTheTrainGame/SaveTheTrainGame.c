/*
Shree
Project:-  SaveTheTrainGame
Author:- Nikhil Ganesh Sathe.
Made between 11 Saur Magh 1946 to 16 Saur Magh 1946.(Indian National Calendar)
31 January 2025 to 5 February 2025 (Gregorian Calendar)

Game was made in response to opengl demo by Raster Group of Astromedicomp's RTR6 Batch on the occaisain of Nikhil Ganesh Sathe's Birthday.

Game is made on Arch Linux with GCC Compiler using freeglut. 
Kernal Version 6.12.10-zen1-1-zen 
*/


// header files
#include <stdio.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>
#include <GL/glu.h>
#include <time.h>

#M_PI 3.14159265359

// global variable declarations
int bFullScreen = 0;
float fTrainPistonMovX = 0.0f;
float fTrainPistonMovY = 0.0f;
float fPistonObjAngle = 0.00f; 
float riverMovX = 0.0f;
float riverWaveSpeed = 0.005f;
int iFlagWon = 0;

void DrumFittings(float fSpawnAt[2], float fScaleBy);
void DrumRims(float fSpawnAt[2], float fScaleBy);
void Chimney(float fSpawnAt[2], float fScaleBy);
void SteamDoms(float fSpawnAt[2], float fScaleBy);
void DrumBars(float fSpawnAt[2], float fScaleBy);
void Drum(float fSpawnAt[2], float fScaleBy);
void WheelPiston(float fSpawnAt[2], float fScaleBy);
void drawcircle(float x, float y, float radius_vert, float radius_hor, float r, float g, float b, float a, int fill, float fScaleBy);

void BackQuad();
void SunRise(float fScaleBy);

// Game state variables
#define MAX_UFOS 1
#define MAX_BOMBS 20
#define UFO_SCALE 0.4f
#define UFO_SPEED 0.008f
#define BOMB_SCALE 0.03f
#define BOMB_DROP_CHANCE 1.7
#define MAX_BULLETS 20
#define TRAIN_SCALE 0.3f
#define TRAIN_START_X 1.5f
#define TRAIN_END_X -1.5f
#define TRAIN_Y -0.6f
#define MAX_LIVES 3
#define BULLET_SPEED 0.02f
#define MAX_TRIANGLES 10
#define SCORE_INCREMENT 10

typedef struct {
    float x;
    float y;
    float speed;
    int active;
    int direction;
} UFO;

typedef struct {
    float x;
    float y;
    float speed;
    int active;
} Bomb;

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    int active;
} Bullet;

UFO ufos[MAX_UFOS];
Bomb bombs[MAX_BOMBS];
Bullet bullets[MAX_BULLETS];
int UFO_light_color = 0;

int score = 0;
int gameActive = 1;
float gunAngle = 0.0f;

float fGunSpawnAt[2] = {0.0f, -0.4f};


// Joystick state
float joystickX = 0.0f;
float joystickY = -0.8f;
float currentJoystickX = 0.0f;
float currentJoystickY = 0.0f;
int isJoystickActive = 0;

int lives = MAX_LIVES;
float trainPosition = TRAIN_START_X;
float trainSpeed = 0.001f;


void drawcircle(float x, float y, float radius_vert, float radius_hor, float r, float g, float b, float a, int fill, float fScaleBy)
{
    float ObjX = 0.0f;
    float ObjY = 0.0f;
    float ObjAngle = 0.0f;

    // Circle
    if (fill == 1) {
        glBegin(GL_POLYGON);
    }
    else {
        glPointSize(5.0f * fScaleBy);
        glBegin(GL_POINTS);
    }

    for (ObjAngle = (0.00f); ObjAngle <= (2.0f * M_PI); ObjAngle += 0.01f)

    {
        ObjX = (x + radius_hor * sin(ObjAngle)) * fScaleBy;
        ObjY = (y + radius_vert * cos(ObjAngle)) * fScaleBy;

        glColor4f(r, g, b, a);

        glVertex2f(ObjX, ObjY);
    }
    glEnd();

}



void drawJoystick() {
    // Base circle
    drawcircle(joystickX, joystickY, 0.15f, 0.15f, 0.8f, 0.8f, 0.8f, 1.0f, 1, 1.0f);

    // Control stick
    if(isJoystickActive) {
        float smallRadius = 0.15f * 0.3f;
        drawcircle(currentJoystickX, currentJoystickY, smallRadius, smallRadius, 0.6f, 0.6f, 0.6f, 1.0f, 1, 1.0f);
    }
}

void initBullets() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = 0;
    }
}

void shootBullet(float fSpawnAt[2], float fScaleBy) {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(!bullets[i].active) {
            float cosTheta = cos(gunAngle * M_PI / 180.0f);
            float sinTheta = sin(gunAngle * M_PI / 180.0f);
            
            // Start bullet at the tip of the gun, considering spawn position and scale
            bullets[i].active = 1;
            bullets[i].x = fSpawnAt[0] + (0.15f * cosTheta * fScaleBy);
            bullets[i].y = fSpawnAt[1] + (0.15f * sinTheta * fScaleBy);
            
            // Calculate direction based on gun angle
            bullets[i].dx = BULLET_SPEED * cosTheta * fScaleBy;
            bullets[i].dy = BULLET_SPEED * sinTheta * fScaleBy;
            break;
        }
    }
}

void updateBullets() {
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            bullets[i].x += bullets[i].dx;
            bullets[i].y += bullets[i].dy;
            
            // Check if bullet is out of boundss
            if(bullets[i].x < -1.0f || bullets[i].x > 1.0f ||
               bullets[i].y < -1.0f || bullets[i].y > 1.0f) {
                bullets[i].active = 0;
                continue;
            }
        }
    }
}

void drawBullets(float fSpawnAt[2], float fScaleBy) {
    glColor3f(1.0f, 1.0f, 0.0f);
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            glBegin(GL_TRIANGLE_FAN);
            glVertex2f(bullets[i].x, bullets[i].y);
            float bulletRadius = 0.02f * fScaleBy;
            for(int j = 0; j <= 360; j += 10) {
                float angle = j * M_PI / 180.0f;
                glVertex2f(bullets[i].x + bulletRadius * cos(angle),
                          bullets[i].y + bulletRadius * sin(angle));
            }
            glEnd();
        }
    }
}

void drawGun(float fSpawnAt[2], float fScaleBy) {
    float x = fSpawnAt[0];
    float y = fSpawnAt[1];
    
    float cosTheta = cos(gunAngle * M_PI / 180.0f);
    float sinTheta = sin(gunAngle * M_PI / 180.0f);
    
    // Gun barrel - rotated rectangle
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    // Calculates rotated vertices
    float x1 = x + (-0.05f * cosTheta - (-0.1f) * sinTheta) * fScaleBy;
    float y1 = y + (-0.05f * sinTheta + (-0.1f) * cosTheta) * fScaleBy;
    
    float x2 = x + (0.05f * cosTheta - (-0.1f) * sinTheta) * fScaleBy;
    float y2 = y + (0.05f * sinTheta + (-0.1f) * cosTheta) * fScaleBy;
    
    float x3 = x + (0.05f * cosTheta - 0.1f * sinTheta) * fScaleBy;
    float y3 = y + (0.05f * sinTheta + 0.1f * cosTheta) * fScaleBy;
    
    float x4 = x + (-0.05f * cosTheta - 0.1f * sinTheta) * fScaleBy;
    float y4 = y + (-0.05f * sinTheta + 0.1f * cosTheta) * fScaleBy;
    
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
    

    glColor3f(0.3f, 0.3f, 0.3f);
    drawcircle(x, y, 0.1f * fScaleBy, 0.1f * fScaleBy, 0.3f, 0.3f, 0.3f, 1.0f, 1, fScaleBy);
    
    // Direction pointer triangle - rotated
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    float tipX = x + (0.15f * cosTheta) * fScaleBy;
    float tipY = y + (0.15f * sinTheta) * fScaleBy;
    
    float base1X = x + (0.1f * cosTheta - 0.03f * sinTheta) * fScaleBy;
    float base1Y = y + (0.1f * sinTheta + 0.03f * cosTheta) * fScaleBy;
    
    float base2X = x + (0.1f * cosTheta + 0.03f * sinTheta) * fScaleBy;
    float base2Y = y + (0.1f * sinTheta - 0.03f * cosTheta) * fScaleBy;
    
    glVertex2f(tipX, tipY);
    glVertex2f(base1X, base1Y);
    glVertex2f(base2X, base2Y);
    glEnd();
}

void handleShootingSystem(float fSpawnAt[2], float fScaleBy) {
        
    // Draw existing bullets
    drawBullets(fSpawnAt, fScaleBy-0.5f);

    // Draw the gun
    drawGun(fSpawnAt, fScaleBy);
    
    // Update bullet positions
    updateBullets();
}

void initBombs() {
    for(int i = 0; i < MAX_BOMBS; i++) {
        bombs[i].active = 0;
    }
}

void spawnBomb(float x, float y) {
    for(int i = 0; i < MAX_BOMBS; i++) {
        if(!bombs[i].active) {
            bombs[i].x = x;
            bombs[i].y = y;
            bombs[i].speed = 0.006f;
            bombs[i].active = 1;
            break;
        }
    }
}

void updateBombs() {
    for(int i = 0; i < MAX_BOMBS; i++) {
        if(bombs[i].active) {
            bombs[i].y -= bombs[i].speed;
            if(bombs[i].y < -1.2f) {
                bombs[i].active = 0;
            }
        }
    }
}

void drawBomb(float x, float y) {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y - BOMB_SCALE);
    glVertex2f(x - BOMB_SCALE, y);
    glVertex2f(x + BOMB_SCALE, y);
    glEnd();
}

void initUFOs() {
    srand(time(NULL));
    for (int i = 0; i < MAX_UFOS; i++) {
        ufos[i].active = 0;
    }
}

void updateUFOs() {
    // one UFO is always active
    if (!ufos[0].active) {
        ufos[0].direction = (rand() % 2) * 2 - 1; // -1 or 1
        ufos[0].x = (ufos[0].direction == 1) ? -1.2f : 1.2f;
        ufos[0].y = 0.8f;
        ufos[0].speed = UFO_SPEED;
        ufos[0].active = 1;
    }
    
    if (ufos[0].active) {
        ufos[0].x += ufos[0].speed * ufos[0].direction;
        
        if (ufos[0].x > 1.2f) {
            ufos[0].direction = -1;
        } else if (ufos[0].x < -1.2f) {
            ufos[0].direction = 1;
        }
        
        // Randomly drop bombs
        if (rand() % 100 < BOMB_DROP_CHANCE) {
            spawnBomb(ufos[0].x, ufos[0].y);
        }
    }
}

void checkCollisions() {
    float trainHitboxX = trainPosition;
    float trainHitboxY = TRAIN_Y;
    float trainHitboxWidth = 0.6f * TRAIN_SCALE;
    float trainHitboxHeight = 0.2f * TRAIN_SCALE;

    // bullet collisions with bombs
    for(int i = 0; i < MAX_BULLETS; i++) {
        if(bullets[i].active) {
            for(int j = 0; j < MAX_BOMBS; j++) {
                if(bombs[j].active) {
                    float dx = bullets[i].x - bombs[j].x;
                    float dy = bullets[i].y - bombs[j].y;
                    float collisionRadius = BOMB_SCALE;
                    
                    if(dx * dx + dy * dy < collisionRadius * collisionRadius) {
                        bombs[j].active = 0;
                        bullets[i].active = 0;
                        score += SCORE_INCREMENT;
                        break;
                    }
                }
            }
        }
    }

    // bomb collisions with train
    for(int i = 0; i < MAX_BOMBS; i++) {
        if(bombs[i].active) {
            float dx = fabs(bombs[i].x - trainHitboxX);
            float dy = fabs(bombs[i].y - trainHitboxY);
            
            if(dx < (trainHitboxWidth + BOMB_SCALE) && 
               dy < (trainHitboxHeight + BOMB_SCALE)) {
                bombs[i].active = 0;
                lives--;
                if(lives <= 0) {
                    gameActive = 0;
                    iFlagWon = 0;
                    printf("Game Over! Train Destroyed! Final Score: %d\n", score);
                }
            }
        }
    }
    
    if (!gameActive) {
        iFlagWon = (lives > 0);
    }
}

void mouse(int button, int state, int x, int y) {
    float mouseX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
    float mouseY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
    
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && gameActive) {
        shootBullet(fGunSpawnAt, 1.0f);
    }
    
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            float dx = mouseX - joystickX;
            float dy = mouseY - joystickY;
            if(dx * dx + dy * dy < 0.15f * 0.15f) {
                isJoystickActive = 1;
                currentJoystickX = mouseX;
                currentJoystickY = mouseY;
            }
        } else if(state == GLUT_UP) {
            isJoystickActive = 0;
            currentJoystickX = joystickX;
            currentJoystickY = joystickY;
            gunAngle = 0.0f;
        }
    }
}

void motion(int x, int y) {
    if(isJoystickActive) {
        float mouseX = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
        float mouseY = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
        
        float dx = mouseX - joystickX;
        float dy = mouseY - joystickY;
        float distance = sqrt(dx * dx + dy * dy);
        
        if(distance > 0.15f) {
            dx = (dx / distance) * 0.15f;
            dy = (dy / distance) * 0.15f;
            currentJoystickX = joystickX + dx;
            currentJoystickY = joystickY + dy;
        } else {
            currentJoystickX = mouseX;
            currentJoystickY = mouseY;
        }
        
        gunAngle = atan2(currentJoystickY - joystickY, 
                        currentJoystickX - joystickX) * 180.0f / M_PI;
    }
}

void keyboard(unsigned char key, int x, int y) {
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

// Entry-point function
int main(int argc, char* argv[])
{
    // local variable declarations
    int width = 1000;
    int height = 1000;
    // local function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    
    void Animate(int);

    // code
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("NikhilSathe's Train Defense Game = RTR018");

    initialize();

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutCloseFunc(uninitialize);
    glutTimerFunc(0,Animate,0);

    initUFOs();
    initBombs();
    initBullets();

    glutMainLoop();

    return(0);
}

int initialize(void)
{
    // code
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.706f, 0.231f, 0.157f, 1.0f);
    return(0);
}

void resize(int width, int height)
{
    // code
    glViewport(0, 0, width, height);
}

void DrumFittings(float fSpawnAt[2], float fScaleBy)
{
    glBegin(GL_LINES);
    glColor3f(0.1f,0.1f,0.1f);
    glVertex2f((-0.425f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.388f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glColor3f(0.706f,0.231f,0.157f);
    glVertex2f((-0.313f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.277f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.11f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.074f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.095f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.131f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.333f * fScaleBy) + fSpawnAt[0], (0.08f * fScaleBy) + fSpawnAt[1]);
    glEnd();
}

void DrumRims(float fSpawnAt[2], float fScaleBy)
{
    glBegin(GL_LINES);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((-0.394f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.394f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.189f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.189f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.015f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.015f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.22f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.22f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glEnd();
}

void Chimney(float fSpawnAt[2], float fScaleBy)
{
    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((-0.32f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.265f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.265f * fScaleBy) + fSpawnAt[0], (0.378f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.32f * fScaleBy) + fSpawnAt[0], (0.378f * fScaleBy) + fSpawnAt[1]);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((-0.32f * fScaleBy) + fSpawnAt[0], (0.378f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.343158f * fScaleBy) + fSpawnAt[0], (0.4f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.241842f * fScaleBy) + fSpawnAt[0], (0.4f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.265f * fScaleBy) + fSpawnAt[0], (0.378f * fScaleBy) + fSpawnAt[1]);
    glEnd();
}

void SteamDoms(float fSpawnAt[2], float fScaleBy)
{
    glLineWidth(2.0f * fScaleBy);
    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((-0.141286f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.141286f * fScaleBy) + fSpawnAt[0], (0.225f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.13f * fScaleBy) + fSpawnAt[0], (0.236f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.056f * fScaleBy) + fSpawnAt[0], (0.236f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.0437f * fScaleBy) + fSpawnAt[0], (0.225f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.0437f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glEnd();
}

void DrumBars(float fSpawnAt[2], float fScaleBy)
{
    glLineWidth(2.0f * fScaleBy);
    glBegin(GL_LINES);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (0.0f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (0.0f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (-0.034f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (-0.034f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (-0.074f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (-0.074f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.51f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.426f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.426f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.35f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.35f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.18f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.18f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.025f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.025f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.14f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.14f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.3f * fScaleBy) + fSpawnAt[0], (0.04f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.295f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glEnd();
}

void Drum(float fSpawnAt[2], float fScaleBy)
{
    glLineWidth(2.0f * fScaleBy);
    // Engine Drum
    glBegin(GL_POLYGON);
    glColor3f(0.0f,0.031f,0.035f);
    
    glVertex2f((-0.45f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.42f * fScaleBy) + fSpawnAt[0], (0.175f * fScaleBy) + fSpawnAt[1]);

    glVertex2f((0.42f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.45f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    
    // Front Line
    // x=1.57\cdot\left(y-0.04\right)^{2}-0.47\cdot\left\{-0.105<y<0.17\right\}
    float x = 0.0f;
    for (float y = -0.105f; y < 0.17f; y+=0.001f)
    {
        x = (1.57*pow((y - 0.035), 2)-0.48); 
        glVertex2f((x * fScaleBy) + fSpawnAt[0], (y * fScaleBy) + fSpawnAt[1]);
    }
    glEnd();

    // Engine Front
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((0.42f * fScaleBy) + fSpawnAt[0], (0.0f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.71f * fScaleBy) + fSpawnAt[0], (0.0f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.71f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.42f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glEnd();

    // Engine Roof
    glBegin(GL_QUADS);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((0.40f * fScaleBy) + fSpawnAt[0], (0.26f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.75f * fScaleBy) + fSpawnAt[0], (0.26f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.75f * fScaleBy) + fSpawnAt[0], (0.22f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.40f * fScaleBy) + fSpawnAt[0], (0.22f * fScaleBy) + fSpawnAt[1]);
    glEnd();

    // Engine Side Bars
    glBegin(GL_LINES);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((0.42f * fScaleBy) + fSpawnAt[0], (0.22f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.42f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.71f * fScaleBy) + fSpawnAt[0], (0.22f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.71f * fScaleBy) + fSpawnAt[0], (-0.105f * fScaleBy) + fSpawnAt[1]);
    glEnd();

    // Drum Rims
    DrumRims(fSpawnAt, fScaleBy);

    // Drum Fittings
    DrumFittings(fSpawnAt, fScaleBy);
    
    // Engine/Drum Bars
    DrumBars(fSpawnAt, fScaleBy);

    // Steam Doms
    SteamDoms(fSpawnAt, fScaleBy);
    
    // Chimney
    Chimney(fSpawnAt, fScaleBy);
}

void WheelPiston(float fSpawnAt[2], float fScaleBy)
{
    float fEndWheelCenter[2] = {0.526f, -0.130f}; 
    float fFirstWheelCenter[2] = {-0.337f, -0.130f}; 

    // Wheels - Scale the radius but keep centers at original positions
    drawcircle(
        (fEndWheelCenter[0] * fScaleBy) + fSpawnAt[0], 
        (fEndWheelCenter[1] * fScaleBy) + fSpawnAt[1], 
        0.10f * fScaleBy, 
        0.10f * fScaleBy, 
        0.0f, 0.031f, 0.035f, 1.0f, 0, 1.0f
    );
    drawcircle(
        (0.246f * fScaleBy) + fSpawnAt[0], 
        (-0.130f * fScaleBy) + fSpawnAt[1], 
        0.10f * fScaleBy, 
        0.10f * fScaleBy, 
        0.0f, 0.031f, 0.035f, 1.0f, 0, 1.0f
    );
    drawcircle(
        (-0.043f * fScaleBy) + fSpawnAt[0], 
        (-0.130f * fScaleBy) + fSpawnAt[1], 
        0.10f * fScaleBy, 
        0.10f * fScaleBy, 
        0.0f, 0.031f, 0.035f, 1.0f, 0, 1.0f
    );
    drawcircle(
        (fFirstWheelCenter[0] * fScaleBy) + fSpawnAt[0], 
        (fFirstWheelCenter[1] * fScaleBy) + fSpawnAt[1], 
        0.10f * fScaleBy, 
        0.10f * fScaleBy, 
        0.0f, 0.031f, 0.035f, 1.0f, 0, 1.0f
    );

    // Wheel Common bar
    glBegin(GL_LINES);
    glColor3f(0.0f,0.031f,0.035f);
    glLineWidth(9.0f * fScaleBy);
    
    float scaledMovX = fTrainPistonMovX * fScaleBy;
    float scaledMovY = fTrainPistonMovY * fScaleBy;
    
    glVertex2f(
        ((fFirstWheelCenter[0] * fScaleBy) - scaledMovX) + fSpawnAt[0], 
        ((fFirstWheelCenter[1] * fScaleBy) - scaledMovY) + fSpawnAt[1]
    );
    glVertex2f(
        ((fEndWheelCenter[0] * fScaleBy) - scaledMovX) + fSpawnAt[0], 
        ((fEndWheelCenter[1] * fScaleBy) - scaledMovY) + fSpawnAt[1]
    );
    glEnd();
 
    glBegin(GL_LINES);
    glLineWidth(9.0f * fScaleBy);
    glVertex2f(
        ((-0.337f * fScaleBy) - scaledMovX) + fSpawnAt[0], 
        (-0.075f * fScaleBy) + fSpawnAt[1]
    );
    glVertex2f(
        (((fEndWheelCenter[0] + fFirstWheelCenter[1]) * fScaleBy / 2.0f) - scaledMovX) + fSpawnAt[0], 
        ((fEndWheelCenter[1] * fScaleBy) - scaledMovY) + fSpawnAt[1]
    );
    glEnd();

    // Wheel Centers
    glBegin(GL_POINTS);
    glColor3f(0.0f,0.031f,0.035f);
    glVertex2f((0.526f * fScaleBy) + fSpawnAt[0], (-0.130f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((0.246f * fScaleBy) + fSpawnAt[0], (-0.130f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.043f * fScaleBy) + fSpawnAt[0], (-0.130f * fScaleBy) + fSpawnAt[1]);
    glVertex2f((-0.337f * fScaleBy) + fSpawnAt[0], (-0.130f * fScaleBy) + fSpawnAt[1]);
    glEnd();
}

void BackQuad()
{
    glBegin(GL_QUADS);
	glColor3f(0.808, 0.318 , 0.071);
    glVertex2f(-1.0f, +1.0f); 
    glVertex2f(+1.0f, +1.0f); 
	glColor3f(0.149f,  0.227f , 0.318f);
    glVertex2f(+1.0f, -1.0f); 
    glVertex2f(-1.0f, -1.0f); 
    glEnd();
}
void SunRise(float fScaleBy)
{
    BackQuad();
    drawcircle(0, 0 , 0.50f, 0.50f, 1.0f, 0.584f, 0.220f, 0.025f, 1, fScaleBy);
    drawcircle(0, 0 , 0.37f, 0.37f, 0.8f, 0.800f, 0.800f, 0.015f, 1, fScaleBy);
    drawcircle(0, 0 , 0.23f, 0.23f, 1.0f, 0.584f, 0.220f, 0.5f, 1, fScaleBy);
    drawcircle(0, 0 , 0.22f, 0.22f, 1.0f, 0.831f, 0.639f, 0.5f, 1, fScaleBy);
    drawcircle(0, 0 , 0.20f, 0.20f, 1.0f, 0.925f, 0.839f, 0.5f, 1, fScaleBy);
}

void Train(float fSpawnAt[2], float fScaleBy)
{
    Drum(fSpawnAt, fScaleBy);
    WheelPiston(fSpawnAt, fScaleBy);
}

void drawRiver(float fSpawnAt[2], float fScaleBy) {
    glBegin(GL_QUAD_STRIP);
    glColor3f(0.0f, 0.4f, 0.8f);
    
    float riverY = -0.5f * fScaleBy + fSpawnAt[1];
    float amplitude = 0.03f * fScaleBy;
    float wavelength = 5.0f / fScaleBy;
    
    // Water Blue
    for (float x = -1.0f * fScaleBy + fSpawnAt[0]; x <= 1.0f * fScaleBy + fSpawnAt[0]; x += 0.01f * fScaleBy) {
        float y = riverY + amplitude * sin(wavelength * (x - fSpawnAt[0]) / fScaleBy + riverMovX);

        glVertex2f(x, y);
        glVertex2f(x, riverY - 0.15f * fScaleBy);
    }
    glEnd();
    
    // Water Reflection white
    glBegin(GL_QUAD_STRIP);
    glColor4f(0.6f, 0.8f, 1.0f, 0.3f);
    for (float x = -1.0f * fScaleBy + fSpawnAt[0]; x <= 1.0f * fScaleBy + fSpawnAt[0]; x += 0.01f * fScaleBy) {
        float y = riverY + amplitude * 0.5f * sin(wavelength * (x - fSpawnAt[0]) / fScaleBy + riverMovX + M_PI);
        y = y - 0.05f * fScaleBy;
        glVertex2f(x, y);
        glVertex2f(x, y - 0.02f);
    }
    glEnd();
}

void drawTree(float x, float y, float scale) {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.02f * scale, y);
    glVertex2f(x + 0.02f * scale, y);
    glVertex2f(x + 0.02f * scale, y + 0.1f * scale);
    glVertex2f(x - 0.02f * scale, y + 0.1f * scale);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.08f * scale, y + 0.08f * scale);
    glVertex2f(x + 0.08f * scale, y + 0.08f * scale);
    glVertex2f(x, y + 0.28f * scale);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.08f * scale, y - 0.00f * scale);
    glVertex2f(x + 0.08f * scale, y - 0.00f * scale);
    glEnd();
}

void drawHills() {

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);

    glVertex2f(-1.0f, -0.3f);
    glVertex2f(-0.8f, 0.1f);
    glVertex2f(-0.6f, -0.3f);
    
    glVertex2f(-0.5f, -0.3f);
    glVertex2f(-0.3f, 0.2f);
    glVertex2f(-0.1f, -0.3f);
    
    glVertex2f(0.0f, -0.3f);
    glVertex2f(0.3f, 0.15f);
    glVertex2f(0.5f, -0.3f);
    
    glVertex2f(0.6f, -0.3f);
    glVertex2f(0.8f, 0.1f);
    glVertex2f(1.0f, -0.3f);
    glEnd();
}

void drawRailwayTrack() {

    glColor3f(0.867, 0.655, 0.337);
    // Rungs 
    for(float x = -1.0f; x <= 1.0f; x += 0.1f) {
        glBegin(GL_LINES);
        glVertex2f(x, TRAIN_Y - 0.135f);
        glVertex2f(x, TRAIN_Y - 0.035f);
        glEnd();
    }

    glColor3f(0.3f, 0.3f, 0.3f);
    
    // Bottom rail
    glBegin(GL_LINES);
    glVertex2f(-1.0f, TRAIN_Y - 0.12f);
    glVertex2f(1.0f, TRAIN_Y - 0.12f);
    
    // Top rail
    glVertex2f(-1.0f, TRAIN_Y - 0.05f);
    glVertex2f(1.0f, TRAIN_Y - 0.05f);
    glEnd();
    
    
}

void drawGrassLand()
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.3f);
    glVertex2f(1.0f, -0.3f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();
}

void drawLives() {
    char livesText[32];
    sprintf(livesText, "Lives: %d", lives);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(0.7f, 0.9f);
    for (char* c = livesText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void displayEndMessage() {
    glColor3f(1.0f, 1.0f, 1.0f);

    const char* message;
    if (iFlagWon) {
        message = "Thank You for Saving Train full of gifts!";
        glColor3f(0.0f, 1.0f, 0.0f);
    } else {
        message = "Please Try Again! Don't Let the Train Get Destroyed!";
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    
    glRasterPos2f(0.0f, 0.2f);
    for(const char* c = message; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    char scoreText[50];
    sprintf(scoreText, "Final Score: %d", score);
    glRasterPos2f(0.0f, 0.0f);
    for(char* c = scoreText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    char livesText[50];
    sprintf(livesText, "Lives Remaining: %d", lives);
    glRasterPos2f(0.0f, -0.2f);
    for(char* c = livesText; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void train()
{
    float fTrainSpawnAt[2] = {trainPosition, TRAIN_Y-0.05f};

    Drum(fTrainSpawnAt, TRAIN_SCALE);
    WheelPiston(fTrainSpawnAt, TRAIN_SCALE);
}
void drawUFOs(void)
{
    for (int i = 0; i < MAX_UFOS; i++) {
        if (ufos[i].active) {
            float rad_cockpit = 0.15;
            float rad_body_vert = 0.12;
            float rad_body_horz = 0.39;

            rad_cockpit = rad_cockpit * UFO_SCALE;
            rad_body_vert = rad_body_vert * UFO_SCALE;
            rad_body_horz = rad_body_horz * UFO_SCALE;

            drawcircle(ufos[i].x, ufos[i].y, rad_cockpit, rad_cockpit, 0.3f, 0.5f, 0.5f, 1.0f, 1, 1.0f);
            drawcircle(ufos[i].x, ufos[i].y-(0.1*UFO_SCALE), rad_body_vert, rad_body_horz, 0.5f, 0.9f, 0.5f, 1.0f, 1, 1.0f);

            drawcircle(ufos[i].x, ufos[i].y-(0.15*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 1.0f, 0.9f, 0.5f, 1.0f, 1, 1.0f);

            if (UFO_light_color == 0) {
                // Lights pattern 1
                drawcircle(ufos[i].x-(0.35*UFO_SCALE), ufos[i].y-(0.09*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 0.0f, 0.671f, 0.059f, 1.0f, 1, 1.0f);
                drawcircle(ufos[i].x+(0.35*UFO_SCALE), ufos[i].y-(0.09*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 0.0f, 0.671f, 0.059f, 1.0f, 1, 1.0f);
                drawcircle(ufos[i].x-(0.15*UFO_SCALE), ufos[i].y-(0.12*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 1.0f, 0.0f, 0.0f, 1.0f, 1, 1.0f);
                drawcircle(ufos[i].x+(0.15*UFO_SCALE), ufos[i].y-(0.12*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 1.0f, 0.0f, 0.0f, 1.0f, 1, 1.0f);
            } else {
                // Lights pattern 2
                drawcircle(ufos[i].x-(0.35*UFO_SCALE), ufos[i].y-(0.09*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 1.0f, 0.0f, 0.0f, 1.0f, 1, 1.0f);
                drawcircle(ufos[i].x+(0.35*UFO_SCALE), ufos[i].y-(0.09*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 1.0f, 0.0f, 0.0f, 1.0f, 1, 1.0f);
                drawcircle(ufos[i].x-(0.15*UFO_SCALE), ufos[i].y-(0.12*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 0.0f, 0.671f, 0.059f, 1.0f, 1, 1.0f);
                drawcircle(ufos[i].x+(0.15*UFO_SCALE), ufos[i].y-(0.12*UFO_SCALE), 0.015f*UFO_SCALE, 0.015f*UFO_SCALE, 0.0f, 0.671f, 0.059f, 1.0f, 1, 1.0f);
            }
        }
    }
    
}
void display(void)
{
    float fRiver1SpawnAt[2] = {0.f, 0.26f};
    float fRiver2SpawnAt[2] = {0.0f, 0.05f};
    float fRiver3SpawnAt[2] = {0.0f, 0.1f};

    
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (gameActive) {
        updateUFOs();
        updateBombs();
        updateBullets();
        checkCollisions();
        
        trainPosition -= trainSpeed;
        if(trainPosition <= TRAIN_END_X) {
            gameActive = 0;
            iFlagWon = 1;
            printf("Victory! Train Successfully Protected! Final Score: %d\n", score);
        }
    }

    SunRise(1.0f);
    drawHills();
    drawGrassLand();
    drawRiver(fRiver1SpawnAt, 1.0f);

    drawTree(-0.7f, -0.6f, 1.3f);
    drawTree(-0.4f, -0.6f, 1.65f);
    drawTree(0.2f, -0.6f, 1.5f);
    drawTree(0.7f, -0.6f, 1.2f);
    
    drawRailwayTrack();
    
    train();

    handleShootingSystem(fGunSpawnAt, 1.0f);
    drawJoystick();
    
    drawUFOs();

    for (int i = 0; i < MAX_BOMBS; i++) {
        if (bombs[i].active) {
            drawBomb(bombs[i].x, bombs[i].y);
        }
    }
    
    if (gameActive) {
        char scoreText[32];
        sprintf(scoreText, "Score: %d", score);
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(-0.9f, 0.9f);
        for (char* c = scoreText; *c != '\0'; c++) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }
        drawLives();
    } else {
        displayEndMessage();
    }
    
    glutSwapBuffers();
}

void Animate(int value)
{
    fTrainPistonMovX += 0.001f;
    if (fPistonObjAngle <= (2.0f * M_PI)) {
        fPistonObjAngle -= 0.06f;
        fTrainPistonMovX = 0.025f * sin(fPistonObjAngle);
        fTrainPistonMovY = 0.025f * cos(fPistonObjAngle);
    }
    if (fPistonObjAngle >= (2.0f * M_PI)) {
        fPistonObjAngle = 0.0f;
    }
    
    // river wave
    riverMovX += riverWaveSpeed;
    if (riverMovX > 2.0f * M_PI) {
        riverMovX = 0.0f;
    }
    
    // UFO lights
    static int frameCount = 0;
    frameCount++;
    if(frameCount % 30 == 0) {
        UFO_light_color = !UFO_light_color;
    }
    
    
    glutTimerFunc(10, Animate, 0);
    glutPostRedisplay();
}
