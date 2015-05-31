#include <stdio.h>
#include <time.h>

#include <GL/glut.h>

#include "core.h"
#include "input.h"
#include "jumping_logic.h"
#include "jumping_draw.h"
#include "geometry.h"
#include "procedural.h"

#define INFO_HEIGHT 30

void init(void);
void initLight(void);
void mainDisplay(void);
void infoDisplay(void);
void mainReshape(int width, int height);
void infoReshape(int width, int height);
void idle(void);

int mainWin;
int infoWin;

int main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(getWidth(), getHeight());

	mainWin = glutCreateWindow("Frogger");
	glutDisplayFunc(mainDisplay);
	glutReshapeFunc(mainReshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutSpecialFunc(specialKeys);
	glutIdleFunc(idle);

	infoWin =
		glutCreateSubWindow(mainWin, 0, 0, getWidth(), INFO_HEIGHT); 
	glutDisplayFunc(infoDisplay); 
	glutReshapeFunc(infoReshape); 

	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}

void init(void)
{
	glutSetWindow(mainWin);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	resetGame();
	updateCartesian(&frog.r);
	updateCartesian(&frog.r0);

	initGrid();
	initSphere();
	initCube();
	initCylinder();
	initCars();
	initLogs();
	initLight();
}

void initLight(void)
{
	float specular[] = {1, 1, 1, 1};
	float shininess[] = {50};
	float lPosition[] = {1, 1, 1, 0};
	float ambient[] = {0, 0, 0, 1};
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, lPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void mainDisplay(void)
{
	glutSetWindow(mainWin);
	glutShowWindow(); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setProjectionMatrix();
	setupCamera();
	GLfloat lightpos[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

	if (getWireFlag())
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (getLightFlag())
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	drawGrid();
	drawDirectionSpeedVector();
	drawParabola();
	drawParabolaNormalTangent();
	drawFrog();
	drawCars();
	drawLogs();

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

void infoDisplay(void)
{
	glutSetWindow(infoWin); 
	glClearColor(0.0, 0.25, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.5, 0.5, 0.5);
	drawText("Lifes <3 <3 <3", -0.95, -0.35, 0);
	drawText("Score: 10", (getWidth()/500) - 0.35, -0.35, 0);
	glutSwapBuffers();
}

void mainReshape(int width, int height)
{
	glutSetWindow(mainWin);
	glViewport(frog.r.x, frog.r.y, width, height);
	setProjectionMatrix();

	infoReshape(width, height);
	glutPostRedisplay();

	setWidth(width);
	setHeight(height);
}

void infoReshape(int width, int height)
{
	glutSetWindow(infoWin); 
	glutReshapeWindow(width, INFO_HEIGHT); 
	glutPositionWindow(0, 0); 
	glutSetWindow(mainWin);
}

void idle(void)
{
	glutSetWindow(mainWin);
	int jump = jumpingIdle();
	if (!getPause())
	{
		updateCars();
		updateLogs();
		glutPostRedisplay();
	}
	else if (jump == 0)
		glutPostRedisplay();
}

