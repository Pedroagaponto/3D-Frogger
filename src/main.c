#include <stdio.h>
#include <time.h>
#include <string.h>

#include <GL/glut.h>

#include "core.h"
#include "input.h"
#include "jumping_logic.h"
#include "jumping_draw.h"
#include "geometry.h"
#include "procedural.h"

#define SUB_HEIGHT 30
#define GAME_OVER_W 200
#define GAME_OVER_H 160

void inputEvents(void);
void init(void);
void initLight(void);
void mainDisplay(void);
void infoDisplay(void);
void osdDisplay(void);
void overDisplay(void);
void mainReshape(int width, int height);
void infoReshape(int width, int height);
void osdReshape(int width, int height);
void overReshape(int width, int height);
void idle(void);

int mainWin;
int infoWin;
int osdWin;
int overWin;

int main(int argc, char **argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(getWidth(), getHeight());

	mainWin = glutCreateWindow("Frogger");
	glutDisplayFunc(mainDisplay);
	glutReshapeFunc(mainReshape);
	inputEvents();
	glutIdleFunc(idle);

	infoWin = glutCreateSubWindow(mainWin, 0, 0, getWidth(), SUB_HEIGHT); 
	glutDisplayFunc(infoDisplay); 
	inputEvents();

	osdWin = glutCreateSubWindow(mainWin, 150, SUB_HEIGHT, 100, SUB_HEIGHT); 
	glutDisplayFunc(osdDisplay); 
	inputEvents();

	overWin = glutCreateSubWindow(mainWin, 0, 0, GAME_OVER_W, GAME_OVER_H); 
	glutDisplayFunc(overDisplay); 
	glutHideWindow();
	glutKeyboardFunc(keyboard);

	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}

void inputEvents(void)
{
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(passiveMouseMove);
	glutSpecialFunc(specialKeys);
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

	initGrid();
	initCylinder();
	initCars();
	initLogs();
	initLight();
	initFrog();
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
	incFrameCount();
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	drawGrid();
	if (!getGameMode())
	{
		drawDirectionSpeedVector();
		drawParabola();
		drawParabolaNormalTangent();
	}
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

	if (getGameMode())
		glutWarpPointer(getWidth()/2, getHeight()/2);
}

void infoDisplay(void)
{
	char buffer[40];

	glutSetWindow(infoWin); 
	glutShowWindow(); 
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	memset(buffer, '\0', 40);
	snprintf(buffer, 40, "Lifes:");
	for (int i = 0; i < getLifes(); i++)
		snprintf(buffer + strlen(buffer), 40, " <3");
	glColor3f(0.5, 0.5, 0.5);
	drawText(buffer, (500/(float)getWidth())*0.07-1, -0.35, 0,
			GLUT_BITMAP_HELVETICA_18);
	memset(buffer, '\0', 40);
	snprintf(buffer, 40, "Score: %3d", getScore());
	drawText(buffer, 1-(500/(float)getWidth())*0.37, -0.35, 0,
			GLUT_BITMAP_HELVETICA_18);

	glutSwapBuffers();
}

void osdDisplay(void)
{
	char buffer[20];

	glutSetWindow(osdWin); 
	glutShowWindow(); 
	glClearColor(0.5, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	memset(buffer, '\0', 20);
	snprintf(buffer, 20, "FPS: %2d", getFps());
	drawText(buffer, -0.26, -0.35, 0, GLUT_BITMAP_HELVETICA_18);

	glutSwapBuffers();
}

void overDisplay(void)
{
	char buffer[20];

	glutSetWindow(overWin); 
	if (!getGameOver())
	{
		glutHideWindow(); 
		return;
	}

	glutShowWindow(); 
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	memset(buffer, '\0', 20);
	snprintf(buffer, 20, "FINAL SCORE: %2d", getScore());
	drawText("GAME OVER", -0.7, 0.2, 0, GLUT_BITMAP_TIMES_ROMAN_24);
	drawText(buffer, -0.65, -0.1, 0, GLUT_BITMAP_9_BY_15);
	drawText("Press [space] to", -0.6, -0.3, 0, GLUT_BITMAP_8_BY_13);
	drawText("restart", -0.25, -0.45, 0, GLUT_BITMAP_8_BY_13);

	glutSwapBuffers();
}

void mainReshape(int width, int height)
{
	glutSetWindow(mainWin);
	glViewport(frog.r.y, frog.r.y, width, height);
	setProjectionMatrix();

	infoReshape(width, height);
	osdReshape(width, height);
	overReshape(width, height);

	setWidth(width);
	setHeight(height);
	glutPostRedisplay();
}

void infoReshape(int width, int height)
{
	glutSetWindow(infoWin);
	glViewport(0, 0, width, height);
	glutReshapeWindow(width, SUB_HEIGHT); 
	glutPositionWindow(0, 0); 
	glutSetWindow(mainWin);
}

void osdReshape(int width, int height)
{
	glutSetWindow(osdWin);
	glViewport(0, 0, width, height); 
	glutReshapeWindow(200, SUB_HEIGHT); 
	glutPositionWindow(width/2-100, SUB_HEIGHT);
	glutSetWindow(mainWin);
}

void overReshape(int width, int height)
{
	glutSetWindow(overWin);
	glViewport(0, 0, width, height); 
	glutReshapeWindow(GAME_OVER_W, GAME_OVER_H); 
	glutPositionWindow(width/2 - GAME_OVER_W/2, height/2 - GAME_OVER_H/2);
	glutSetWindow(mainWin);
}

void idle(void)
{
	calculatePerformance();
	glutSetWindow(mainWin);
	int jump = jumpingIdle();
	if (!getPause())
	{
		updateCars();
		updateLogs();

		if (gameCheck())
		{
			if (getLifes() > 1)
			{
				setLifes(getLifes()-1);
				resetGame();
			}
			else
			{
				setGameOver(true);
				overDisplay();
			}
		}
		gameCheck();
		glutPostRedisplay();
	}
	else if (jump == 0)
		glutPostRedisplay();
}

