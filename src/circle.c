#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#include "core.h"
#include "circle.h"

void drawParametricCircle(void);
void drawCartesianCircle(void);
void drawCircleParametricTangents(void);
void drawCircleParametricNormals(void);
void drawCircleCartesianTangents(void);
void drawCircleCartesianNormals(void);

static const float cRadius =  0.05;

void drawCircle(void)
{
	if (getCartesianFlag())
		drawCartesianCircle();
	else
		drawParametricCircle();
}

/****** LOCAL FUNCTIONS ******/
void drawParametricCircle(void)
{
	if (getDebug())
		printf(">>>>PARAMETRIC CIRCLE<<<<<\n");
	glBegin(GL_LINE_LOOP);
	glColor3f (0.8, 0.8, 0.8);
	for (int i = 0; i < getSegments(); i++)
	{
		//parametric equations for a circle
		//x = r.cos(t)
		//y = r.sin(t)
		float t = (i / (float) getSegments()) * 2 * M_PI;
		glVertex3f(cRadius*cos(t)+frog.r.x, cRadius*sin(t)+frog.r.y, frog.r.z);
		if (getDebug())
			printf("Circle vertex coordenates: (%f, %f), t = %f\n",
			cRadius*cos(t)+frog.r.x, cRadius*sin(t)+frog.r.y, t+frog.r.z);
	}
	glEnd();

	if (getTangentFlag())
		drawCircleParametricTangents();
	if (getNormalFlag())
		drawCircleParametricNormals();
}

/* For a circle which has its centre at (frog.r.x, frog.r.y):
 * (x - frog.r.x)^2 + (y - frog.r.y)^2 = cRadius^2
 * it can be rearranged and solved for y to give:
 * y = +-sqrt(cRadius^2-x^2)
 */
void drawCartesianCircle(void)
{
	if (getDebug())
		printf(">>>>CARTESIAN CIRCLE<<<<<\n");
	int halfSeg = getSegments()/2;
	glBegin(GL_LINE_LOOP);
	glColor3f (0.8, 0.8, 0.8);
	for (int i = 0; i < halfSeg; i++)
	{
		//-sqrt(cRadius^2-x^2)
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		glVertex3f(x+frog.r.x, -sqrt(cRadius*cRadius-x*x)+frog.r.y, frog.r.z);
		if (getDebug())
			printf("Circle vertex coordenates: (%f, %f)\n",
					x + frog.r.x, -sqrt(cRadius*cRadius-x*x)+frog.r.y);
	}

	for (int i = halfSeg; i > 0 ; i--)
	{
		//+sqrt(cRadius^2-x^2)
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		glVertex3f(x + frog.r.x, sqrt(cRadius*cRadius-x*x)+frog.r.y, frog.r.z);
		if (getDebug())
			printf("Circle vertex coordenates: (%f, %f)\n",
					x + frog.r.x, sqrt(cRadius*cRadius-x*x)+frog.r.y);
	}
	glEnd();

	if (getTangentFlag())
		drawCircleCartesianTangents();
	if (getNormalFlag())
		drawCircleCartesianNormals();
}

/* Given that tan = dy/dx
 * is possible to use this on the parametric equations for a circle
 * so, tan = <dx/dt, dy/dt> = <-cRadius*sin(t), cRdius*cos(t)>
 */
void drawCircleParametricTangents(void)
{
	for (int i = 0; i < getSegments(); i++)
	{
		glBegin(GL_LINES);
		glColor3f (0, 1, 1);
		float t = (i / (float) getSegments()) * 2 * M_PI;
		float x = cRadius*cos(t);
		float y = cRadius*sin(t);

		glVertex3f(x + frog.r.x, y + frog.r.y, frog.r.z);
		float n = sqrt(y*y + x*x)*REDUCTION;
		glVertex3f(-y/n+x + frog.r.x, x/n+y + frog.r.y, frog.r.z);

		glEnd();
	}
}

/* Given that N.T = 0, and T = <dx/dt, dy/dt>
 * N = <dy/dt, -dx/dt>
 * so, N = <cRadius*cos(t), cRadius*sin(t)>
 */
void drawCircleParametricNormals(void)
{
	for (int i = 0; i < getSegments(); i++)
	{
		glBegin(GL_LINES);
		glColor3f (1, 1, 0);
		float t = (i / (float) getSegments()) * 2 * M_PI;
		float x = cRadius*cos(t);
		float y = cRadius*sin(t);

		glVertex3f(x + frog.r.x, y + frog.r.y, frog.r.z);
		float n = sqrt(y*y + x*x)*REDUCTION;
		glVertex3f(x + x/n + frog.r.x, y + y/n + frog.r.y, frog.r.z);

		glEnd();
	}
}

/* Given that tan = dy/dx and y = sqrt(cRadius^2-x^2)
 * dy/dx = -x/y
 * so, tan = <-y, x>
 */
void drawCircleCartesianTangents(void)
{
	int halfSeg = getSegments()/2;
	for (int i = 0; i <= halfSeg; i++)
	{
		glBegin(GL_LINES);
		glColor3f (0, 1, 1);
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		float y1 = -sqrt(cRadius*cRadius-x*x);
		float y2 = sqrt(cRadius*cRadius-x*x);

		glVertex3f(x + frog.r.x, y1 + frog.r.y, frog.r.z);
		float n = sqrt(y1*y1 + x*x)*REDUCTION;
		glVertex3f(x - y1/n + frog.r.x, y1 + x/n + frog.r.y, frog.r.z);

		glVertex3f(x + frog.r.x, y2 + frog.r.y, frog.r.z);
		n = sqrt(y2*y2 + x*x)*REDUCTION;
		glVertex3f(x - y2/n + frog.r.x, y2 + x/n + frog.r.y, frog.r.z);

		glEnd();
	}
}

/* Given that N.T = 0, T = <dx/dt, dy/dt> and dy/dx = -x/
 * N = <x, y>
 */
void drawCircleCartesianNormals(void)
{
	int halfSeg = getSegments()/2;
	for (int i = 0; i <= halfSeg; i++)
	{
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		float y1 = -sqrt(cRadius*cRadius-x*x);
		float y2 = sqrt(cRadius*cRadius-x*x);

		glVertex3f(x + frog.r.x, y1 + frog.r.y, frog.r.z);
		float n = sqrt(y1*y1 + x*x)*REDUCTION;
		glVertex3f(x + x/n + frog.r.x, y1 + y1/n + frog.r.y, frog.r.z);

		glVertex3f(x + frog.r.x, y2 + frog.r.y, frog.r.z);
		n = sqrt(y2*y2 + x*x)*REDUCTION;
		glVertex3f(x + x/n + frog.r.x, y2 + y2/n + frog.r.y, frog.r.z);

		glEnd();
	}
}

