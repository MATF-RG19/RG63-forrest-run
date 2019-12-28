#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>

float step_s = 0.0f;

static int window_width, window_height;

static float camPosX = 1;
static float camPosY = 0.5;
static float camPosZ = 2;

static float kx = 0.999605f;
static float kz = 0.012737f;

#define PI 3.14159
#define MOUSE_SENSITIVITY 0.005

static int SHOULD_HANDLE_MOUSE_MOVEMENT = 0;

static float x_angle = 0;
static float y_angle = 0;

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
static void on_mouse_motion(int x, int y);

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
    glutPassiveMotionFunc(on_mouse_motion);

    glClearColor(0.098f, 0.098f, 0.43f, 0);
    glEnable(GL_DEPTH_TEST);

    // Light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glutMainLoop();
    return 0;
}

static void on_mouse_motion(int x, int y)
{
    if (!SHOULD_HANDLE_MOUSE_MOVEMENT)
    {
        return;
    }

    x -= window_width / 2;
    y -= window_height / 2;

    x_angle += y * MOUSE_SENSITIVITY;
    y_angle += x * MOUSE_SENSITIVITY;

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
        0.05f, 7);                           // how close and how far do we see

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ,           // where's our camera (eye)
              camPosX + kx, camPosY, camPosZ + kz, // what are we looking at
              0, 1, 0);                            // what's our up vector

    // Cube 1
    glPushMatrix();
    setMaterialForRedCube();
    glTranslatef(0, 0.15, 0);
    glutSolidCube(0.3);
    glPopMatrix();

    // Cube 2
    glPushMatrix();
    setMaterialForRedCube();
    glTranslatef(0.3, 0.1, -0.3);
    glutSolidCube(0.2);
    glPopMatrix();

    drawTree(0.8, 0, 0.4);

    drawTree(-1, 0, -0.4);

    drawTree(2.5, 0, 1.25);

    // Grass
    glPushMatrix();
    setMaterialForGrass();
    glScalef(10, 0.01, 10);
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
        SHOULD_HANDLE_MOUSE_MOVEMENT = 1;
        break;
    case 'w':
        step_s = sqrt(kz * kz + kx * kx);
        camPosX += kx / step_s * 0.03f; // scale down
        camPosZ += kz / step_s * 0.03f; // scale down
        break;
    case 's':
        step_s = sqrt(kz * kz + kx * kx);
        camPosX -= kx / step_s * 0.03f; // scale down
        camPosZ -= kz / step_s * 0.03f; // scale down
    default:
        break;
    }
    // force rerender
    glutPostRedisplay();
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}

void setMaterialForGrass()
{
    GLfloat ambient_coeffs[] = {0.05, 0.4, 0.05, 0.2};
    GLfloat diffuse_coeffs[] = {0.1, 0.9, 0.1, 0.9};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

void setMaterialForRedCube()
{
    GLfloat ambient_coeffs[] = {0.9, 0.05, 0.05, 0.2};
    GLfloat diffuse_coeffs[] = {0.9, 0.05, 0.05, 0.9};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

void setMaterialForDarkBrownWood()
{
    GLfloat ambient_coeffs[] = {0.40, 0.26, 0.13, 0.01};
    GLfloat diffuse_coeffs[] = {0.40, 0.26, 0.13, 0.3};
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
    glutPostRedisplay();
}

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
    drawCone(coordX, coordY + 0.3, coordZ, 0.4, 1);
}

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