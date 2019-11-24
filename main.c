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

static float prev_mouse_x = 0;
static float prev_mouse_y = 0.5;

static int should_handle_mouse_movement = 0;
static int is_first_mouse_movement_handling = 1;

static void on_reshape(int width, int height);
static void on_display(void);
static void on_mouse_movement(int x, int y);
static void on_keyboard(unsigned char key, int x, int y);

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
    glutPassiveMotionFunc(on_mouse_movement);
    glutKeyboardFunc(on_keyboard);

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

static void on_mouse_movement(int x, int y)
{
    if (should_handle_mouse_movement)
    {
        if ((float)window_width - (float)x < (float)window_width / 50) // poslednjih 2% ekrana (desna ivica)
        {
            lookingAtX += 80 / ((float)window_width);
        }
        if ((float)x < (float)window_width / 50) // poslednjih 2% ekrana (leva ivica)
        {
            lookingAtX -= 80 / ((float)window_width);
        }
        lookingAtX -= 2 * ((float)prev_mouse_x - (float)x) / ((float)window_width);
        lookingAtY += 2 * ((float)prev_mouse_y - (float)y) / ((float)window_height);
    }
    prev_mouse_x = (float)x;
    prev_mouse_y = (float)y;
    glutPostRedisplay();
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'w':
        nX += lookingAtX * 0.1f;
        // TODO dodaj y deo
        nZ -= 0.1f;
        break;
    case 's':
        nX -= lookingAtX * 0.1f;
        // TODO dodaj y deo
        nZ += 0.1f;
        break;
    case 'z':
        should_handle_mouse_movement = 1;
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