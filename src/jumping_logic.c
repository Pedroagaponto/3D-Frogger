#include <stdio.h>

#include <GL/glut.h>

#include "jumping_logic.h"
#include "jumping_draw.h"
#include "core.h"

void calcPositionAnalytical(float t);
void calcPositionNumerical(float dt);

static float startTime = 0;

static bool jumpingFlag = false;
static bool analyticFlag = true;

bool getJumpingFlag(void)
{
	return jumpingFlag;
}

void switchAnalyticFlag(void)
{
	analyticFlag = !analyticFlag;
}

/*
 * Function executed when space key is pressed,
 * and just allow jumping inside the window
 */
void jumpingSettings(void)
{
	if (!jumpingFlag && parabolaInsideWindow())
	{
		jumpingFlag = true;
		startTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		frog.r.angle = frog.r0.angle;
		frog.r.speed = frog.r0.speed;
		glutPostRedisplay();
	}
}

int jumpingIdle(void)
{
	static float tLast = -1.0;
	float t, dt;

	if (!jumpingFlag)
		return -1;

	t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - startTime;

	if (tLast < 0.0)
	{
		tLast = t;
		return -1;
	}

	dt = t - tLast;

	if (getDebug())
		printf(">>>>>JUMP<<<<<\nt=%f\tstartTime=%f\tx=%f\ty=%f\tz=%f\n",
				t, startTime, frog.r.x, frog.r.y, frog.r.z);
	calcPosition(t, dt);

	if (!jumpingFlag)
		tLast = -1;
	else
		tLast = t;

	return 0;
}

/* Function responsible for calculating how far the frog will go */
float calcReach(void)
{
	return ((frog.r0.speed*frog.r0.speed) / GRAVITY) * sin(2*frog.r0.angle);
}

void calcPosition(float t, float dt)
{
	if (analyticFlag)
		calcPositionAnalytical(t);
	else
		calcPositionNumerical(dt);
}

/* Using the calcReach, checks if will land inside the window */
bool parabolaInsideWindow(void)
{
	if ((calcReach() + frog.r0.x < 1.0 + frog.r.x) && (calcReach() + frog.r0.x > -1.0 + frog.r.x))
		return true;

	return false;
}

/****** LOCAL FUNCTIONS ******/
void calcPositionAnalytical(float t)
{
	float tEnd = (2*(frog.r0.speed*sin(frog.r0.angle)))/(GRAVITY);
	/* If the t is greater than the projected landing tEnd, sets t as tEnd so
	 * it lands exact on the right position, not going a bit further if the
	 * loop took a bit longer */
	if (t > tEnd)
		t = tEnd;
	float x = frog.r0.speed * t * cos(frog.r0.angle);
	frog.r.x = frog.r0.x + x;

	if (getCartesianFlag())
		frog.r.y = frog.r0.y + tan(frog.r0.angle)*x-(GRAVITY*x*x)/
			(2 * pow(cos(frog.r0.angle)*frog.r0.speed,2));
	else
		frog.r.y = frog.r0.speed * t * sin(frog.r0.angle) -
			(GRAVITY*t*t)/2 + frog.r0.y;

	if (getDebug())
		printf("Analytical Jump: angle %f, speed %f, x %f, y %f\n",
				frog.r.angle*180/M_PI, frog.r.speed, frog.r.x, frog.r.y);
	if (t >= tEnd)
	{
		jumpingFlag = false;
		frog.r0.x = frog.r.x;
		frog.r0.y = frog.r.y;
	}

	float dx = frog.r0.speed * cos(frog.r0.angle);
	float dy = frog.r0.speed * sin(frog.r0.angle) - GRAVITY*t;
	frog.r.speed = sqrt(dx*dx + dy*dy);
	frog.r.angle = atan(dy/dx);
	if (dx < 0)
		frog.r.angle+=M_PI;
}

void calcPositionNumerical(float dt)
{
	float speedX = frog.r.speed * cos(frog.r.angle);
	float speedY = frog.r.speed * sin(frog.r.angle);
	frog.r.x += speedX * dt;
	frog.r.y += speedY * dt;

	speedY += -GRAVITY * dt;
	frog.r.speed = sqrt(speedX*speedX + speedY*speedY);
	frog.r.angle = atan(speedY/speedX);
	if (speedX < 0)
		frog.r.angle+=M_PI;

	if (getDebug())
		printf("Numerical Jump: angle %f, speed %f, x %f, y %f\n",
				frog.r.angle*180/M_PI, frog.r.speed, frog.r.x, frog.r.y);
	if (frog.r.y < frog.r0.y)
	{
		jumpingFlag = false;
		frog.r.y = frog.r0.y;
		frog.r0.x+= calcReach();
		frog.r.x = frog.r0.x;
	}
}

