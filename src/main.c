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

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);

	updateCartesian(&frog.r);
	updateCartesian(&frog.r0);
	initGrid();
	initSphere();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	setProjectionMatrix();
	setupCamera();

	if (getWireFlag())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawAxes();
	drawGrid();
	drawDirectionSpeedVector();
	drawParabola();
	drawParabolaNormalTangent();
	drawSphere();
	glutSwapBuffers();
	if (getDebug())
	{
		printf("frog\tx\ty\tz\tr\ttheta\tphi\tdx\tdy\tdz\n"
			   "r0:\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n"
			   "r :\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n",
			   frog.r0.x,  frog.r0.y,     frog.r0.z,
			   frog.r0.r,  frog.r0.theta, frog.r0.phi,
			   frog.r0.dx, frog.r0.dy,    frog.r0.dz,
			   frog.r.x,   frog.r.y,      frog.r.z,
			   frog.r.r,   frog.r.theta,  frog.r.phi,
			   frog.r.dx,  frog.r.dy,     frog.r.dz);
		printf("\n\n");
	}
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

