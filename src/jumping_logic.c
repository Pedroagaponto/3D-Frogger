#include <stdio.h>

#include <GL/glut.h>

#include "jumping_logic.h"
#include "jumping_draw.h"
#include "core.h"

void calcPositionNumerical(float dt);
void calcPositionAnalytical(float t);

static float startTime = 0;
static bool analyticFlag = true;

void switchAnalyticFlag(void)
{
	analyticFlag = !analyticFlag;
}

bool collisionDetection(void)
{
	for (int i = 0; i < OBSTACLE_SIZE; i++)
		for (int j = 0; j < LINE_OBSTACLES; j++)
		{
			if ((frog.r.x == logs[i][j].x ) && (frog.r.y == 0) &&
					(frog.r.z > logs[i][j].z - CYLINDER_HEIGHT/2) &&
					(frog.r.z < logs[i][j].z + CYLINDER_HEIGHT/2))
			{
				if (getDebug())
					printf("On top of log\n");
				
				frog.onLog = true;

				if (i % 2 == 0)
					frog.logDirection = 1;
				else
					frog.logDirection = -1;

				return false;
			}
			if ((frog.r.x >= cars[i][j].x - CUBE_LENGTH/2) &&
					(frog.r.x <= cars[i][j].x + CUBE_LENGTH/2) &&
					(frog.r.y <= CUBE_LENGTH) &&
					(frog.r.y >= 0) &&
					(frog.r.z >= cars[i][j].z - CUBE_LENGTH/2) &&
					(frog.r.z <= cars[i][j].z + CUBE_LENGTH/2))
			{
				if (getDebug())
					printf("Frog/car colision\n");
				return true;
			}
		}

	frog.onLog = false; 

	int riverInit =
		(GRASS_SIZE+INTERVAL_SIZE+OBSTACLE_SIZE)*LINE_WIDTH+LINE_WIDTH/2;
	if ((frog.r.y == 0) && (frog.r.x >= riverInit) &&
			(frog.r.x <= riverInit + LINE_WIDTH * 4))
		return true;

	return false;
}

/*
 * Function executed when a jumping key is pressed,
 * and just allow jumping inside the stablished limit
 */
void jumpingSettings(void)
{
	if (!getJumpingFlag() && parabolaInsideWindow())
	{
		setJumpingFlag(true);
		startTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
		frog.r.theta = frog.r0.theta;
		frog.r.r = frog.r0.r;
		frog.r.phi = frog.r0.phi;
		updateCartesian(&frog.r);
		glutPostRedisplay();
	}
}

int jumpingIdle(void)
{
	static float tLast = -1.0;
	float t, dt;

	if (!getJumpingFlag())
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
	calcPositionNumerical(dt);

	if (!getJumpingFlag())
		tLast = -1;
	else
		tLast = t;

	return 0;
}

/* Function responsible for calculating how far the frog will go */
float calcReach(void)
{
	return ((frog.r0.r*frog.r0.r) / GRAVITY) * sin(2*frog.r0.theta);
}

/* Function responsible for calculating how far the frog will go */
float calcTimeOfFlight(void)
{
	return (2*(frog.r0.r * cos(frog.r0.theta)))/(GRAVITY);
}

/* Using the calcReach, checks if will land inside the window */
bool parabolaInsideWindow(void)
{
	if (calcReach() < 10.0)
	{
		float reachX = frog.r0.x + calcReach() * sin(frog.r0.phi);
		float reachZ = frog.r0.z + calcReach() * cos(frog.r0.phi);

		if ((reachX >= 0 && reachX <= GRID_WIDTH * LINE_WIDTH) &&
			(reachZ >= 0 && reachZ <= GRID_HEIGHT * LINE_WIDTH))
			return true;
	}
	return false;
}

/****** LOCAL FUNCTIONS ******/
void calcPositionNumerical(float dt)
{
	frog.r.x += frog.r.dx * dt;
	frog.r.z += frog.r.dz * dt;
	frog.r.y += frog.r.dy * dt;

	if (getVerletFlag())
	{
		frog.r.y+=(GRAVITY / 2) * dt*dt;
		if (getDebug())
			printf("(Jump calculed using Velocity Verlet Integration)\n");
	}
	else if (getDebug())
		printf("(Jump calculed using Euler Integration)\n");

	frog.r.dy += -GRAVITY * dt;

	updateSpherical(&frog.r);

	if (getDebug())
		printf("Numerical Jump: angle %f, speed %f, x %f, y %f, z %f\n",
				frog.r.theta*180/M_PI, frog.r.r, frog.r.x, frog.r.y, frog.r.dz);
	if (frog.r.y <= 0)
	{
		frog.r.y = frog.r0.y = 0;
		frog.r0.x += calcReach() * sin(frog.r0.phi);
		frog.r0.z += calcReach() * cos(frog.r0.phi);
		frog.r.x = frog.r0.x;
		frog.r.z = frog.r0.z;
		setJumpingFlag(false);
		updateSpherical(&frog.r0);
		frog.r = frog.r0;
		glutPostRedisplay();
	}
}

void calcPositionAnalytical(float t)
{
	UNUSED_VAR t;
#ifdef DISABLE_FUNCTION
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
		getJumpingFlag() = false;
		frog.r0.x = frog.r.x;
		frog.r0.y = frog.r.y;
	}

	float dx = frog.r0.speed * cos(frog.r0.angle);
	float dy = frog.r0.speed * sin(frog.r0.angle) - GRAVITY*t;
	frog.r.speed = sqrt(dx*dx + dy*dy);
	frog.r.angle = atan(dy/dx);
	if (dx < 0)
		frog.r.angle+=M_PI;
#endif
}

