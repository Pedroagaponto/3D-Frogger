#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

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

#ifndef M_PI
#define M_PI 3.14159265359
#endif
#define UNUSED_VAR (void)

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void drawAxes(void);
void drawCircle(void);
void drawParametricCircle(void);
void drawParametricTangent(void);
void drawParametricNormal(void);
void drawCartesianCircle(void);
void drawCartesianTangent(void);
void drawCartesianNormal(void);

typedef struct{
	float x, y;
} vec2f;

typedef struct {
	vec2f r0, v0, r, v;
} frogState;

frogState frog = {
	{ 0.0, 0.0 },
	{ 1.0, 2.0 },
	{ 0.0, 0.0 },
	{ 1.0, 2.0 }
};

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

const int segments = 10;
const float cRadius =  0.05;

bool debug = true;
bool cartesianFlag = true;
bool tangentFlag = true;
bool normalFlag = true;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
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
	drawCircle();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	switch (key)
	{
		case 'f':
		case 'F':
			cartesianFlag = !cartesianFlag;
			break;
		case 'd':
		case 'D':
			debug = !debug;
			break;
		case 't':
		case 'T':
			tangentFlag = !tangentFlag;
			break;
		case 'n':
		case 'N':
			normalFlag = !normalFlag;
			break;
		case 27:
		case 'q':
		case 'Q':
			exit(EXIT_SUCCESS);
		default:
			break;
	}
	glutPostRedisplay();
}

void drawAxes(void)
{
	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_LINES);
		glColor3f(colours[i][0], colours[i][1], colours[i][2]);
		glVertex3f(0, 0, -1);
		glVertex3f(axesPosition[i][0], axesPosition[i][1], axesPosition[i][2]);
		glEnd();
	}

	if (debug)
		printf(">>>>>AXES DREW<<<<<\n\n");
}

void drawCircle(void)
{
	if (cartesianFlag)
		drawCartesianCircle();
	else
		drawParametricCircle();
}

void drawParametricCircle(void)
{
	glBegin(GL_LINE_LOOP);
	glColor3f (0.8, 0.8, 0.8);
	for (int i = 0; i <= segments; i++)
	{
		float t = (i / (float) segments) * 2 * M_PI;
		glVertex3f(cRadius*cos(t)+frog.r.x, cRadius*sin(t)+frog.r.y, 0);
		if (debug)
			printf("Circle vertex coordenates: (%f, %f), t = %f\n",
			cRadius*cos(t)+frog.r.x, cRadius*sin(t)+frog.r.y, t);
	}
	glEnd();

	if (debug)
		printf(">>>>PARAMETRIC CIRCLE DREW<<<<<\n\n");
	if (tangentFlag)
		drawParametricTangent();
	if (normalFlag)
		drawParametricNormal();
}

void drawParametricTangent(void)
{
	for (int i = 0; i <= segments; i++)
	{
		glBegin(GL_LINES);
		glColor3f (0, 1, 1);
		float t = (i / (float) segments) * 2 * M_PI;
		float x = cRadius*cos(t)+frog.r.x;
		float y = cRadius*sin(t)+frog.r.y;
		glVertex3f(x, y, 0);
		glVertex3f(x - y, y + x, 0);
		glEnd();
	}
}

void drawParametricNormal(void)
{
	for (int i = 0; i <= segments; i++)
	{
		glBegin(GL_LINES);
		glColor3f (1, 1, 0);
		float t = (i / (float) segments) * 2 * M_PI;
		float x = cRadius*cos(t)+frog.r.x;
		float y = cRadius*sin(t)+frog.r.y;
		glVertex3f(x, y, 0);
		glVertex3f(2*x, 2*y, 0);
		glEnd();
	}
}

void drawCartesianCircle(void)
{
	glBegin(GL_LINE_LOOP);
	glColor3f (0.8, 0.8, 0.8);
	for (int i = 0; i <= segments; i++)
	{
		float t = (i / (float) segments) * 2 * cRadius - cRadius;
		float aux = -pow(frog.r.x, 2)+(2*frog.r.x*t)+pow(cRadius, 2)-pow(t, 2);
		glVertex3f(t + frog.r.x, frog.r.y - sqrt(aux), 0);
		if (debug)
			printf("Circle vertex coordenates: (%f, %f), t = %f\n",
					t + frog.r.x, frog.r.y - sqrt(aux), t);
	}

	for (int i = segments; i >= 0 ; i--)
	{
		float t = (i / (float) segments) * 2 * cRadius - cRadius;
		float aux = -pow(frog.r.x, 2)+(2*frog.r.x*t)+pow(cRadius, 2)-pow(t, 2);
		glVertex3f(t + frog.r.x, sqrt(aux) + frog.r.y, 0);
		if (debug)
			printf("Circle vertex coordenates: (%f, %f), t = %f\n",
					t + frog.r.x, sqrt(aux) + frog.r.y, t);
	}
	glEnd();

	if (debug)
		printf(">>>>CARTESIAN CIRCLE DREW<<<<<\n\n");
	if (tangentFlag)
		drawCartesianTangent();
	if (normalFlag)
		drawCartesianNormal();
}

void drawCartesianTangent(void)
{
	for (int i = 0; i <= segments; i++)
	{
		glBegin(GL_LINES);
		glColor3f (0, 1, 1);
		float t = (i / (float) segments) * 2 * cRadius - cRadius;
		float aux = -pow(frog.r.x, 2)+(2*frog.r.x*t)+pow(cRadius, 2)-pow(t, 2);
		float x = t+frog.r.x;
		float y1 = frog.r.y - sqrt(aux);
		float y2 = frog.r.y + sqrt(aux);
		glVertex3f(x, y1, 0);
		glVertex3f(x-y1, y1+x, 0);
		glVertex3f(x, y2, 0);
		glVertex3f(x-y2, y2+x, 0);
		glEnd();
	}
}

void drawCartesianNormal(void)
{
	for (int i = 0; i <= segments; i++)
	{
		glBegin(GL_LINES);
		glColor3f (1, 1, 0);
		float t = (i / (float) segments) * 2 * cRadius - cRadius;
		float aux = -pow(frog.r.x, 2)+(2*frog.r.x*t)+pow(cRadius, 2)-pow(t, 2);
		float x = t+frog.r.x;
		float y1 = frog.r.y - sqrt(aux);
		float y2 = frog.r.y + sqrt(aux);
		glVertex3f(x, y1, 0);
		glVertex3f(2*x, 2*y1, 0);
		glVertex3f(x, y2, 0);
		glVertex3f(2*x, 2*y2, 0);
		glEnd();
	}
}
