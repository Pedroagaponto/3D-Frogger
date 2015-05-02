#include <stdio.h>

#include <GL/glut.h>

#include "jumping_draw.h"
#include "jumping_logic.h"
#include "core.h"

void drawCartesianParabola(void);
void drawParametricParabola(void);

/*
 * Draw the speed vector, the force it will be launched
 * or the speed vector during the jump
 */
void drawDirectionSpeedVector(void)
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 1);
	if (getJumpingFlag())
	{
		glVertex3f(frog.r.x, frog.r.y, frog.r.z);
		glVertex3f(frog.r.speed*0.1*cos(frog.r.angle)+frog.r.x,
				frog.r.speed*0.1*sin(frog.r.angle)+frog.r.y, frog.r.z);
	}
	else
	{
		glVertex3f(frog.r0.x, frog.r0.y, frog.r0.z);
		glVertex3f(frog.r0.speed*0.1*cos(frog.r0.angle)+frog.r0.x,
				frog.r0.speed*0.1*sin(frog.r0.angle)+frog.r0.y, frog.r0.z);
	}
	glEnd();
}

void drawParabola(void)
{
	if (getDebug())
		printf(">>>>>PARABOLA<<<<<\ncalcReach:%f speed:%f angle:%f\n",
				calcReach(), frog.r0.speed, frog.r0.angle*180/M_PI);

	/* Tha parabola change color to red when the frog will land outside */
	if (parabolaInsideWindow())
		glColor3f(0, 0, 1);
	else
		glColor3f(1, 0, 0);

	if (getCartesianFlag())
		drawCartesianParabola();
	else
		drawParametricParabola();
}

void drawParabolaNormalTangent(void)
{
	float x, y, dx, dy, n;
	for (int i = 0; i <= getSegments(); i++)
	{
		if (getCartesianFlag())
		{
			float distance = calcReach();
			x = (i/(float)getSegments())*distance;
			float y1 = 2 * pow(cos(frog.r0.angle)*frog.r0.speed,2);
			y = tan(frog.r0.angle) * x - (GRAVITY*x*x)/y1;
			dy = tan(frog.r0.angle) -
				(GRAVITY*x)/(pow(cos(frog.r0.angle)*frog.r0.speed,2));
			dx = 1;
		}
		else
		{
			float t = (i/(float)getSegments())*
				(2*(frog.r0.speed*sin(frog.r0.angle)))/(GRAVITY);
			x = frog.r0.speed * t * cos(frog.r0.angle);
			y = frog.r0.speed * t * sin(frog.r0.angle) - (GRAVITY*t*t)/2;
			dx = frog.r0.speed * cos(frog.r0.angle);
			dy = frog.r0.speed * sin(frog.r0.angle) - GRAVITY*t;
		}

		if (getTangentFlag())
		{
			glBegin(GL_LINES);
			glColor3f (0, 1, 1);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z);
			n = sqrt(dx*dx + dy*dy)*REDUCTION;
			/* Invert the result, due to the way it is drawn*/
			if (getCartesianFlag() && x < 0)
				glVertex3f(frog.r0.x - dx/n + x, frog.r0.y - dy/n + y, frog.r0.z);
			else
				glVertex3f(frog.r0.x + dx/n + x, frog.r0.y + dy/n + y, frog.r0.z);

			glEnd();
		}

		if (getNormalFlag())
		{
			glBegin(GL_LINES);
			glColor3f (1, 1, 0);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z);
			n = sqrt(dx*dx + dy*dy)*REDUCTION;
			/* Invert the result, due to the way it is drawn*/
			if (!getCartesianFlag() && x < 0)
				glVertex3f(frog.r0.x + dy/n + x, frog.r0.y - dx/n + y, frog.r0.z);
			else
				glVertex3f(frog.r0.x - dy/n + x, frog.r0.y + dx/n + y, frog.r0.z);

			glEnd();
		}
	}
}

/****** LOCAL FUNCTIONS ******/
void drawCartesianParabola(void)
{
	//y = tan(θ) x - g * x^2 / (2 (v*cos(θ))^2)
	glBegin(GL_LINE_STRIP);

	float distance = ((frog.r0.speed*frog.r0.speed) / GRAVITY) *
		sin(2*frog.r0.angle);
	for (int i = 0; i <= getSegments(); i++)
	{
		float x = (i/(float)getSegments())*distance;
		float y1 = 2 * pow(cos(frog.r0.angle)*frog.r0.speed,2);
		float y = tan(frog.r0.angle) * x - (GRAVITY*x*x)/y1;
		if (getDebug())
			printf("drawCartesianParabola: d = %.5f\tx = %.5f\ty = %.5f\n",
					distance, x + frog.r0.x, y + frog.r0.y);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z);
	}
	glEnd();
}

void drawParametricParabola(void)
{
	//x = v t cos(θ)
	//y = v t sin(θ) - 1/2 g t^2
	glBegin(GL_LINE_STRIP);

	float distance = (2*(frog.r0.speed*sin(frog.r0.angle)))/(GRAVITY);
	for (int i = 0; i <= getSegments(); i++)
	{
		float t = (i/(float)getSegments())*distance;
		float x = frog.r0.speed * t * cos(frog.r0.angle);
		float y = frog.r0.speed * t * sin(frog.r0.angle) - (GRAVITY*t*t)/2;
		if (getDebug())
			printf("drawParametricParabola: d = %.5f\tx = %.5f\ty = %.5f\n",
					distance, x + frog.r0.x, y + frog.r0.y);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z);
	}
	glEnd();
}

