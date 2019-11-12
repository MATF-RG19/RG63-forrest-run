#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>

static int window_width, window_height;

static float nX = 0;
static float nY = 0.5;
static float nZ = 2;

static float lookingAtX = 0;
static float lookingAtY = 0.5;
static float lookingAtZ = 0.5;

static void on_reshape(int width, int height);
static void on_display(void);

void setColorRGB(int r, int g, int b);

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    glClearColor(1, 1, 1, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();
    return 0;
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        60,                                  // y-angle
        window_width / (float)window_height, // aspect ratio
        0.05f, 7);                           // how close and how far do we see

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        nX, nY, nZ,                       // where's our camera (eye)
        lookingAtX, lookingAtY, nZ - 1.5, // what are we looking at
        0, 1, 0);                         // what's our up vector

    // Grass
    setColorRGB(11,102,35);
    glBegin(GL_QUADS);
        glPushMatrix();
            glVertex3f(3.0f, 0, -3.0f);
            glVertex3f(-3.0f, 0, -3.0f);
            glVertex3f(-3.0f, 0, 3.0f);
            glVertex3f(3.0f, 0, 3.0f);
        glPopMatrix();
    glEnd();

    // Refresh
    glutSwapBuffers();
}

void setColorRGB(int r, int g, int b)
{
    glColor3f(((float)r) / 255, ((float)g) / 255, ((float)b) / 255);
}

static void on_reshape(int width, int height)
{
    window_width = width;
    window_height = height;
}