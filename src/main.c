#include <stdlib.h>

#if _WIN32
#   include <Windows.h>
#endif
#if __APPLE__
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <GLUT/glut.h>
#else
#   include <GL/gl.h>
#   include <GL/glu.h>
#   include <GL/glut.h>
#endif

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void drawAxes(void);

int colours[][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
}; 

int axesPosition[][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
}; 

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Assignment 1");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return EXIT_SUCCESS;
}

void init(void)
{
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    drawAxes();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
        case 'q':
            exit(EXIT_SUCCESS);
        default:
            break;
    }
}

void drawAxes(void)
{
    for (int i = 0; i < 3; i++)
    {
        glBegin(GL_LINES);
        glColor3f(colours[i][0], colours[i][1], colours[i][2]);
        glVertex3f(0, 0, 0);
        glVertex3f(axesPosition[i][0], axesPosition[i][1], axesPosition[i][2]);
        glEnd();
    }
}
