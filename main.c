#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <math.h>
#include <time.h>
#define GIFT_1_X 3
#define GIFT_1_Z 3
#define GIFT_2_X 0
#define GIFT_2_Z 9
#define PI 3.14159
#define MOUSE_SENSITIVITY 0.005
float step_s = 0.0f; // Sluzi za odredjivanje distance koja se predje pritiskom na 'w' ili 's'
int animation_time_1 = 0; // Za kutiju (poklon) 1
int animation_time_2 = 0; // Za kutiju 2
static int window_width, window_height;
static float camPosX = 1;
static float camPosY = 0.5;
static float camPosZ = 2;

static float kx = 0.999605f; // Pocetna razlika izmedju pozicije posmatraca i predmeta iu koji gleda
static float kz = 0.012737f;

static int SHOULD_HANDLE_MOUSE_MOVEMENT = 0; // Taster 'h' postavlja ovo na 1

static float x_angle = 0; // Udaljenja od x / y ose
static float y_angle = 0;

// Podesavanje komponenti svetla
GLfloat light_ambient[] = {0.7, 0.7, 0.7, 0.4};
GLfloat light_diffuse[] = {0.4, 0.9, 0.4, 0.4};
GLfloat light_specular[] = {0.2, 0.9, 0.2, 0.2};
GLfloat light_position[] = {0, 0, 1, 0};

static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
void drawTree(float coordX, float coordY, float coordZ);
void drawCylinder(float coordX, float coordY, float coordZ, float radius, float height);
void drawCone(float coordX, float coordY, float coordZ, float radius, float height);
void setMaterialForGrass();
void setMaterialForRedCube();
void setMaterialForWhiteCube();
void drawGift(float coordX, float coordY, float coordZ, int animation_param);
void on_mouse_movement(int x, int y);
void on_timer_tick(int timer_key);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Forest run");

    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);
    glutKeyboardFunc(on_keyboard);
    glutPassiveMotionFunc(on_mouse_movement);

    glClearColor(0.098f, 0.098f, 0.43f, 0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glutMainLoop();
    return 0;
}

void on_timer_tick(int key)
{

    // Animacija za kuriju 1 je odvojena od one za kutiju 2
    if (key == 1) {
        animation_time_1 += 10;
    }
     if (key == 2) {
        animation_time_2 += 10;
    }

    glutPostRedisplay();

    // Animacija za kuriju 1 je odvojena od one za kutiju 2
    if (animation_time_1 >= 400)
    {
        animation_time_1 = 0;
        return;
    }
    if (animation_time_2 >= 400)
    {
        animation_time_2 = 0;
        return;
    }

    glutTimerFunc(10, on_timer_tick, key);
}

void on_mouse_movement(int x, int y)
{
    // Pritisnuti taster 'h' da se hendluje pomeraj kursora
    if (!SHOULD_HANDLE_MOUSE_MOVEMENT)
    {
        return;
    }

    x -= window_width / 2;
    y -= window_height / 2;

    // Skaliranje ugla odudaranja od x / y osa na osnovu pomeraja kursora
    x_angle += y * MOUSE_SENSITIVITY;
    y_angle += x * MOUSE_SENSITIVITY;

    // Ne zelimo da ugao ispadne iz intervala
    if (x_angle >= 180)
    {
        x_angle = 180;
    }

    if (x_angle <= 0)
    {
        x_angle = 0;
    }

    kx = cos(PI / 180.0f * y_angle) * sin(PI / 180.0f * x_angle);
    printf("kx %f\n", kx);
    kz = sin(PI / 180.0f * y_angle) * sin(PI / 180.0f * x_angle);
    printf("kz %f\n", kz);

    glutPostRedisplay();
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60,                                  // y-angle
        window_width / (float)window_height, // aspect ratio
        0.05f, 15);                          // how close and how far do we see

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ,           // where's our camera (eye)
              camPosX + kx, camPosY, camPosZ + kz, // what are we looking at
              0, 1, 0);                            // what's our up vector

    drawGift(GIFT_1_X, 0, GIFT_1_Z, animation_time_1);

    drawGift(GIFT_2_X, 0, GIFT_2_Z, animation_time_2);

    // Postavljanje drva na nasumicne pozicije

    drawTree(5, 0, 4);

    drawTree(5.4, 0, 2);

    drawTree(7, 0, 4);

    drawTree(8, 0, 8);

    drawTree(0.8, 0, 0.4);

    drawTree(-1, 0, -0.4);

    drawTree(2.5, 0, 1.25);

    drawTree(-4, 0, -2.25);

    drawTree(-0.8, 0, -3);

    drawTree(1, 0, -0.4);

    drawTree(-2.5, 0, -1.25);

    drawTree(-9, 0, -6.25);

    drawTree(-2.5, 0, 8.25);

    drawTree(-4, 0, 14);

    drawTree(-0.8, 0, 13);

    drawTree(1, 0, 11);

    drawTree(-2.5, 0, 6);

    drawTree(-9, 0, 2);

    drawTree(2.5, 0, 5.8);

    drawTree(4, 0, 6);

    drawTree(0.8, 0, 2.9);

    drawTree(1, 0, 3);

    drawTree(-2.5, 0, 6);

    drawTree(9, 0, 20);

    drawTree(0, 0, 13);

    drawTree(0, 0, 11);

    drawTree(-2, 0, 7);

    drawTree(-5, 0, 7);

    // Grass
    glPushMatrix();
        setMaterialForGrass();
        glScalef(40, 0.01, 40);
        glutSolidCube(1);
    glPopMatrix();

    // Refresh
    glutSwapBuffers();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'h':
        // Dok se taster 'h' ne pritisne, pomeraji kursora ne uticu na pogled
        SHOULD_HANDLE_MOUSE_MOVEMENT = 1;
        break;
    case 'w':
        step_s = sqrt(kz * kz + kx * kx);
        camPosX += kx / step_s * 0.08f; // Mnozenje sa 0.08f smanjuje distancu koju prelazimo pritiskom na taster 'w'
        camPosZ += kz / step_s * 0.08f;
        // Posebno hendlovanje za animaciju kod prvog poklona i kod drugog
        if (camPosX >= GIFT_1_X - 1 && camPosX <= GIFT_1_X + 1 && camPosZ >= GIFT_1_Z - 1 && camPosZ <= GIFT_1_Z + 1)
        {
            glutTimerFunc(10, on_timer_tick, 1);
        }
        if (camPosX >= GIFT_2_X - 1 && camPosX <= GIFT_2_X + 1 && camPosZ >= GIFT_2_Z - 1 && camPosZ <= GIFT_2_Z + 1)
        {
            glutTimerFunc(10, on_timer_tick, 2);
        }
        break;
    case 's':
        step_s = sqrt(kz * kz + kx * kx);
        camPosX -= kx / step_s * 0.06f; // Mnozenje sa 0.06f smanjuje distancu koju prelazimo pritiskom na taster 's'
        camPosZ -= kz / step_s * 0.06f;
        // Posebno hendlovanje za animaciju kod prvog poklona i kod drugog
        if (camPosX >= GIFT_1_X - 1 && camPosX <= GIFT_1_X + 1 && camPosZ >= GIFT_1_Z - 1 && camPosZ <= GIFT_1_Z + 1)
        {
            glutTimerFunc(10, on_timer_tick, 1);
        }
        if (camPosX >= GIFT_2_X - 1 && camPosX <= GIFT_2_X + 1 && camPosZ >= GIFT_2_Z - 1 && camPosZ <= GIFT_2_Z + 1)
        {
            glutTimerFunc(10, on_timer_tick, 2);
        }
    default:
        break;
    }
    glutPostRedisplay();
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}

void drawGift(float coordX, float coordY, float coordZ, int animation_param)
{

    glPushMatrix();
        // Uzletanje kocke kad se okine animacija
        glTranslatef(0, (float)animation_param / 100.0f, 0);
        // Pozicioniranje celog poklona - inicijalno stoji na x = 0, y = h/2, z = 0
        glTranslatef(coordX, coordY, coordZ);
        // Iznigni poklon da njegovo dno bude po defaultu na tlu
        glTranslatef(0, 0.15f, 0);
        // Rotacija poklona oko svoje ose, kada se tajmer upali
        glRotatef(animation_param, 0, 1, 0);

        // Crvena kutija poklona
        glPushMatrix();
            setMaterialForRedCube();
            glutSolidCube(0.3);
        glPopMatrix();

        // Bela traka broj 1
        glPushMatrix();
            setMaterialForWhiteCube();
            glScalef(0.2f, 1.02f, 1.02f);
            glutSolidCube(0.3);
        glPopMatrix();

        // Bela traka broj 2
        glPushMatrix();
            setMaterialForWhiteCube();
            glScalef(1.02f, 1.02f, 0.2f);
            glutSolidCube(0.3);
        glPopMatrix();
    glPopMatrix();
}

// Zelena boja za travu
void setMaterialForGrass()
{
    GLfloat ambient_coeffs[] = {0.05, 0.4, 0.05, 0.2};
    GLfloat diffuse_coeffs[] = {0.1, 0.9, 0.1, 0.9};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

// Za kutiju za poklon
void setMaterialForRedCube()
{
    GLfloat ambient_coeffs[] = {0.9, 0.05, 0.05, 0.2};
    GLfloat diffuse_coeffs[] = {0.9, 0.05, 0.05, 0.9};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

// Bice korisceno za trake na poklonu
void setMaterialForWhiteCube()
{
    GLfloat ambient_coeffs[] = {1, 1, 1, 0.2};
    GLfloat diffuse_coeffs[] = {1, 1, 1, 0.9};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

// Za stablo jelke
void setMaterialForDarkBrownWood()
{
    GLfloat ambient_coeffs[] = {0.40, 0.26, 0.13, 0.01};
    GLfloat diffuse_coeffs[] = {0.40, 0.26, 0.13, 0.3};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

// Zeleno za krosnju jelke
void setMaterialForChristmasTree()
{
    GLfloat ambient_coeffs[] = {0, 0.2, 0, 0.1};
    GLfloat diffuse_coeffs[] = {0, 0.2, 0, 0.7};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

void drawTree(float coordX, float coordY, float coordZ)
{
    setMaterialForDarkBrownWood();
    drawCylinder(coordX, coordY, coordZ, 0.04, 1);

    setMaterialForChristmasTree();
    // Izdigni krosnju od tla
    drawCone(coordX, coordY + 0.3, coordZ, 0.4, 1);
}

// Za krosnju jelke
void drawCone(float coordX, float coordY, float coordZ, float radius, float height)
{
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.6;

    glBegin(GL_TRIANGLES);
    for (angle = 0.0; angle < 360; angle += angle_stepsize)
    {
        GLfloat x = radius * cos(angle) + coordX;
        GLfloat y = radius * sin(angle) + coordZ;
        GLfloat x1 = radius * cos(angle + angle_stepsize) + coordX;
        GLfloat y1 = radius * sin(angle + angle_stepsize) + coordZ;

        // Lower 2 vertices
        glVertex3f(x, coordY, y);
        glVertex3f(x1, coordY, y1);

        // Upper vertex (the tip of the cone)
        glVertex3f(coordX, coordY + height, coordZ);
    }
    glEnd();
}

// Za stablo jelke
void drawCylinder(float coordX, float coordY, float coordZ, float radius, float height)
{
    GLfloat angle = 0.0;
    GLfloat angle_stepsize = 0.6;

    glBegin(GL_QUAD_STRIP);
    for (angle = 0.0; angle < 360; angle += angle_stepsize)
    {
        GLfloat x = radius * cos(angle) + coordX;
        GLfloat y = radius * sin(angle) + coordZ;
        GLfloat x1 = radius * cos(angle + angle_stepsize) + coordX;
        GLfloat y1 = radius * sin(angle + angle_stepsize) + coordZ;

        // Lower 2 vertices
        glVertex3f(x, coordY, y);
        glVertex3f(x1, coordY, y1);

        // Upper 2 vertices
        glVertex3f(x, height + coordY, y);
        glVertex3f(x1, height + coordY, y1);
    }
    glEnd();
}