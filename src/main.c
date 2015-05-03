#include <stdio.h>

#include <GL/glut.h>

#include "core.h"
#include "input.h"
#include "jumping_logic.h"
#include "jumping_draw.h"
#include "sphere.h"
//#include "circle.h"

void init(void);
void display(void);
void idle(void);
void reshape(int width, int height);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(getWidth(), getHeight());
	glutCreateWindow("Assignment 2");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(idle);
	glutMainLoop();

	return EXIT_SUCCESS;
}

void init(void)
{
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	initGrid();
}

void display(void)
{
	float camX = frog.r.x + cos(75);
	float camY = frog.r.y + cos(75);
	float camZ = frog.r.z + cos(75);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (getWireFlag())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	setProjectionMatrix();
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, frog.r.x, frog.r.y, frog.r.z, 0, 1, 0);
	glTranslatef(frog.r.x, frog.r.y, frog.r.z);
	//glRotatef(getRotateCamY(), 0, 1, 0);
	glRotatef(getRotateCamX(), 1, 0, 0);
	glTranslatef(-frog.r.x, -frog.r.y, -frog.r.z);

	drawAxes();
	drawGrid();
	drawDirectionSpeedVector();
	drawParabola();
	drawParabolaNormalTangent();
	drawSphere();
	glutSwapBuffers();
	if (getDebug())
		printf("\n\n");
}

void idle(void)
{
	if (jumpingIdle() == 0)
		glutPostRedisplay();
}

void reshape(int width, int height)
{
	glViewport(frog.r.x, frog.r.y, width, height);
	setProjectionMatrix();
	glutPostRedisplay();
	setWidth(width);
	setHeight(height);
}

