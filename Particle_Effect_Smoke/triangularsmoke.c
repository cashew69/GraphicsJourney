#include <GL/freeglut.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PARTICLES 1000
#define EMISSION_RATE 1    // number of particles emitted per frame
#define INITIAL_LIFESPAN 0.3f

// particle def
typedef struct {
    float x, y;       // Position
    float dx, dy;     // Direction and speed
    float size;       // Size of the particle
    float alpha;      // Transparency
    float lifespan;   // Remaining lifespan
} Particle;

Particle particles[MAX_PARTICLES];
int activeParticles = 0;  

// viman position
float rocketX = 0.0f;
float rocketY = 0.0f;

// initialize a single particle
void initParticle(Particle *p) {

    // Usingp Pointere arrow notation don't get confused.
    p->x = rocketX / 500.0f;  
    p->y = rocketY; 

    // Horizontal drift for coneish effect.
    // Increase denominator to slowdown and vice a versa
    // Change range of 100-50 to make small cone/.
    // Check rand() C reference for more.
    // here is how it works suppose we wrote rand()%100 then it will throw us val between
    // 0 t0 99 and when we remove 50 from it shifts on negative side now range becomes
    // -50 to 49
    // let say i want -25 to 25 then i will do rand()%50-25
    p->dx = (rand() % 50 - 25)/ 10000.0f;
    // similar to dx but here can go up and change speed if needed.
    p->dy = -(rand() % 50 + 50)/ 10000.0f;

    p->size = (rand() % 10) / 1000.0f + 0.0005f;//  change the + 0.0005f value to change size
    p->alpha = 0.5f;
    p->lifespan = INITIAL_LIFESPAN;
}

// particles array initialization
void initParticles() {
    srand(time(0));
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].lifespan = 0.0f;  // marks all particles as inactive initially
    }
}

void updateParticles() {
    // emit new particles at a steady rate
    for (int i = 0; i < EMISSION_RATE; i++) {
        if (activeParticles < MAX_PARTICLES) {
            initParticle(&particles[activeParticles]);
            activeParticles++;
        }
    }

    // update existing particles
    for (int i = 0; i < activeParticles; i++) {
        Particle *p = &particles[i];

        p->x += p->dx;
        p->y += p->dy;
        p->alpha -= 0.01f;       
        p->lifespan -= 0.01f;   

        // resets particle if it "dies"
        if (p->lifespan <= 0 || p->alpha <= 0) {
            initParticle(p);
        }
    }
}

void renderParticles() {
    for (int i = 0; i < activeParticles; i++) {
        Particle *p = &particles[i];

        glColor4f(0.6f, 0.6f, 0.6f, p->alpha); // grey color with alpha
        glBegin(GL_TRIANGLES);
        glVertex2f(p->x, p->y); // yop vertex
        glVertex2f(p->x - p->size, p->y - p->size); // bottom-left
        glVertex2f(p->x + p->size, p->y - p->size);// bottom-right
        glEnd();
    }
}

void renderRocket() {
    glColor3f(1.0f, 1.0f, 0.0f); 
    glBegin(GL_TRIANGLES);
    glVertex2f(rocketX - 0.02f, rocketY + 0.02f);
    glVertex2f(rocketX + 0.02f, rocketY + 0.02f);
    glVertex2f(rocketX, rocketY + 0.1f); 
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    renderParticles();

    renderRocket();

    glutSwapBuffers();
}

void idle() {
    updateParticles();
    glutPostRedisplay();
}

// Handle keyboard input
void keyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        rocketY += 0.02f;
        break;
    case GLUT_KEY_DOWN:
        rocketY -= 0.02f; 
        break;

    }
    glutPostRedisplay();
}

// Initialize OpenGL
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rocket Smoke Particle Effect - NIkhil Sathe");

    initGL();
    initParticles();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutSpecialFunc(keyboard);

    glutMainLoop();
    return 0;
}

