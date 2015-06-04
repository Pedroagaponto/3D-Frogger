#include <stdio.h>

#include <GL/glut.h>

#include "jumping_draw.h"
#include "jumping_logic.h"
#include "core.h"
#include "geometry.h"
#include "new_geometry.h"
#include "objloader.h"

newMesh test;

void drawCartesianParabola(void);
void drawParametricParabola(void);

void initFrog(void)
{
	test.obj = loadTexture("res/frog.jpg");
	load_obj("res/frog.obj", &test);
}

void drawFrog(void)
{
	glColor3f(1, 1, 1);
	glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT | GL_TEXTURE_BIT);
	glBindTexture(GL_TEXTURE_2D, test.obj);
	glTranslatef(frog.r.x, frog.r.y, frog.r.z);
	render_mesh(&test);
	glTranslatef(-frog.r.x, -frog.r.y, -frog.r.z);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopAttrib();
}

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
		glVertex3f(frog.r.dx*0.1 + frog.r.x, frog.r.dy*0.1 + frog.r.y,
				frog.r.dz*0.1 + frog.r.z);
	}
	else
	{
		glVertex3f(frog.r0.x, frog.r0.y, frog.r0.z);
		glVertex3f(frog.r0.dx*0.1 + frog.r0.x, frog.r0.dy*0.1 + frog.r0.y,
				frog.r0.dz*0.1 + frog.r0.z);
	}
	glEnd();
}

void drawParabola(void)
{
	if (getDebug())
		printf(">>>>>PARABOLA DREW<<<<<\ncalcReach:%f speed:%f angle:%f\n",
				calcReach(), frog.r0.r, frog.r0.theta*180/M_PI);

	/* Tha parabola change color to red when the frog will land outside */
	if (parabolaInsideWindow())
		glColor3f(0, 0, 1);
	else
		glColor3f(1, 0, 0);

	drawParametricParabola();
}

void drawParabolaNormalTangent(void)
{
	float x, y, z, dx, dy, dz, n;
	for (int i = 0; i <= getSegments(); i++)
	{
		float t = (i/(float)getSegments()) * calcTimeOfFlight();
		x = t * frog.r0.dx;
		z = t * frog.r0.dz;
		y = t * frog.r0.dy - (GRAVITY*t*t)/2;
		dx = frog.r0.dx;
		dz = frog.r0.dz;
		dy = frog.r0.dy - (GRAVITY*t);

		if (getNormalFlag())
		{
			glBegin(GL_LINES);
			glColor3f (0, 1, 1);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z + z);
			n = sqrt(dx*dx + dy*dy + dz*dz)*REDUCTION;
			glVertex3f(frog.r0.x + dx/n + x, frog.r0.y + dy/n + y, frog.r0.z + dz/n + z);

			glEnd();
		}

#ifdef disabled_function
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
#endif
	}

}

/****** LOCAL FUNCTIONS ******/
void drawCartesianParabola(void)
{
#ifdef DISABLE_FUNCTION
	//y = tan(θ) x - g * x^2 / (2 (v*cos(θ))^2)
	glBegin(GL_LINE_STRIP);

	float distance = calcReach();
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
#endif
}

void drawParametricParabola(void)
{
	//x = v t sin(θ) sin(ϕ)
	//z = v t sin(θ) cos(ϕ)
	//y = v t cos(θ) - 1/2 g t^2
	glBegin(GL_LINE_STRIP);

	float ToF = calcTimeOfFlight();
	for (int i = 0; i <= getSegments(); i++)
	{
		float t = (i/(float)getSegments())*ToF;
		float x = t * frog.r0.dx;
		float z = t * frog.r0.dz;
		float y = t * frog.r0.dy - (GRAVITY*t*t)/2;
		if (getDebug())
			printf("drawParametricParabola: d = %.5f\tx = %.5f\ty = %.5f\tz = %.5f\n",
					ToF, x + frog.r0.x, y + frog.r0.y, z + frog.r0.z);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, frog.r0.z + z);
	}
	glEnd();
}

