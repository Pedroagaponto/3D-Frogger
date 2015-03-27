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
#define REDUCTION 25

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void idle(void);
void drawAxes(void);
void drawCircle(void);
void drawParametricCircle(void);
void drawCircleParametricTangents(void);
void drawCircleParametricNormals(void);
void drawCartesianCircle(void);
void drawCircleCartesianTangents(void);
void drawCircleCartesianNormals(void);
void drawDirectionSpeedVector(void);
void drawCartesianParabola();
void drawParametricParabola();
void drawParabola();
void drawParabolaNormalTangent();
float calcReach(void);
bool parabolaInsideWindow(void);

typedef struct{
	float x, y;
	float speed, angle;
} projection;

typedef struct {
	projection r0, r;
} frogState;

frogState frog = {
	{ 0.0, 0.0, 1.0, M_PI/4 },
	{ 0.0, 0.0, 1.0, M_PI/4 }
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

const float cRadius =  0.05;
const float gravity = 9.8;
//float speed = 1;
//float angle = M_PI/4;

int segments = 10;
float startTime = 0;
bool debug = true;
bool cartesianFlag = true;
bool tangentFlag = true;
bool normalFlag = true;
bool jumpingFlag = false;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Assignment 1");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
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
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	drawAxes();
	drawCircle();
	drawDirectionSpeedVector();
	drawParabola();
	drawParabolaNormalTangent();
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	switch (key)
	{
		case ' ':
			if(jumpingFlag == false && parabolaInsideWindow())
			{
				jumpingFlag = true;
				startTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
			}
			break;
		case 'f':
		case 'F':
			cartesianFlag = !cartesianFlag;
			break;
		case 'v':
		case 'V':
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
		case 'a':
		case 'A':
			frog.r0.angle = (frog.r0.angle < M_PI-M_PI/32) ? frog.r0.angle+M_PI/32 : M_PI-M_PI/32;
			break;
		case 'd':
		case 'D':
			frog.r0.angle = (frog.r0.angle < M_PI/16) ? M_PI/32 : frog.r0.angle-M_PI/32;
			break;
		case 'w':
		case 'W':
			frog.r0.speed = (frog.r0.speed > 9.8) ? 10 : frog.r0.speed+0.2;
			break;
		case 's':
		case 'S':
			frog.r0.speed = (frog.r0.speed < 1.2) ? 1 : frog.r0.speed-0.2;
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

void specialKeys(int key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	switch (key)
	{
		case GLUT_KEY_UP:
			segments *= 2;
			break;
		case GLUT_KEY_DOWN:
			segments = (segments < 8) ? 4 : segments/2;
			break;
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
	for (int i = 0; i < segments; i++)
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
		drawCircleParametricTangents();
	if (normalFlag)
		drawCircleParametricNormals();
}

void drawCircleParametricTangents(void)
{
	for (int i = 0; i < segments; i++)
	{
		glBegin(GL_LINES);
		glColor3f (0, 1, 1);
		float t = (i / (float) segments) * 2 * M_PI;
		float x = cRadius*cos(t);
		float y = cRadius*sin(t);

		glVertex3f(x + frog.r.x, y + frog.r.y, 0);
		float n = sqrt(y*y + x*x)*REDUCTION;
		glVertex3f(-y/n+x + frog.r.x, x/n+y + frog.r.y, 0);

		glEnd();
	}
}

void drawCircleParametricNormals(void)
{
	for (int i = 0; i < segments; i++)
	{
		glBegin(GL_LINES);
		glColor3f (1, 1, 0);
		float t = (i / (float) segments) * 2 * M_PI;
		float x = cRadius*cos(t);
		float y = cRadius*sin(t);

		glVertex3f(x + frog.r.x, y + frog.r.y, 0);
		float n = sqrt(y*y + x*x)*REDUCTION;
		glVertex3f(x + x/n + frog.r.x, y + y/n + frog.r.y, 0);

		glEnd();
	}
}

void drawCartesianCircle(void)
{
	int halfSeg = segments/2;
	glBegin(GL_LINE_LOOP);
	glColor3f (0.8, 0.8, 0.8);
	for (int i = 0; i < halfSeg; i++)
	{
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		glVertex3f(x+frog.r.x, -sqrt(cRadius*cRadius-x*x)+frog.r.y, 0);
		if (debug)
			printf("Circle vertex coordenates: (%f, %f)\n",
					x + frog.r.x, -sqrt(cRadius*cRadius-x*x)+frog.r.y);
	}

	for (int i = halfSeg; i > 0 ; i--)
	{
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		glVertex3f(x + frog.r.x, sqrt(cRadius*cRadius-x*x)+frog.r.y, 0);
		if (debug)
			printf("Circle vertex coordenates: (%f, %f)\n",
					x + frog.r.x, sqrt(cRadius*cRadius-x*x)+frog.r.y);
	}
	glEnd();

	if (debug)
		printf(">>>>CARTESIAN CIRCLE DREW<<<<<\n\n");
	if (tangentFlag)
		drawCircleCartesianTangents();
	if (normalFlag)
		drawCircleCartesianNormals();
}

void drawCircleCartesianTangents(void)
{
	int halfSeg = segments/2;
	for (int i = 0; i <= halfSeg; i++)
	{
		glBegin(GL_LINES);
		glColor3f (0, 1, 1);
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		float y1 = -sqrt(cRadius*cRadius-x*x);
		float y2 = sqrt(cRadius*cRadius-x*x);

		glVertex3f(x + frog.r.x, y1 + frog.r.y, 0);
		float n = sqrt(y1*y1 + x*x)*REDUCTION;
		glVertex3f(x - y1/n + frog.r.x, y1 + x/n + frog.r.y, 0);

		glVertex3f(x + frog.r.x, y2 + frog.r.y, 0);
		n = sqrt(y2*y2 + x*x)*REDUCTION;
		glVertex3f(x - y2/n + frog.r.x, y2 + x/n + frog.r.y, 0);

		glEnd();
	}
}

void drawCircleCartesianNormals(void)
{
	int halfSeg = segments/2;
	for (int i = 0; i <= halfSeg; i++)
	{
		glBegin(GL_LINES);
		glColor3f(1, 1, 0);
		float x = (i / (float) halfSeg) * 2 * cRadius - cRadius;
		float y1 = -sqrt(cRadius*cRadius-x*x);
		float y2 = sqrt(cRadius*cRadius-x*x);

		glVertex3f(x + frog.r.x, y1 + frog.r.y, 0);
		float n = sqrt(y1*y1 + x*x)*REDUCTION;
		glVertex3f(x + x/n + frog.r.x, y1 + y1/n + frog.r.y, 0);

		glVertex3f(x + frog.r.x, y2 + frog.r.y, 0);
		n = sqrt(y2*y2 + x*x)*REDUCTION;
		glVertex3f(x + x/n + frog.r.x, y2 + y2/n + frog.r.y, 0);

		glEnd();
	}
}

void drawDirectionSpeedVector(void)
{
	glBegin(GL_LINES);
	glColor3f(1, 0, 1);
	glVertex3f(frog.r0.x, frog.r0.y, 0);
	glVertex3f(frog.r0.speed*0.1*cos(frog.r0.angle)+frog.r0.x, frog.r0.speed*0.1*sin(frog.r0.angle)+frog.r0.y, 0);
	glEnd();
}

void drawCartesianParabola()
{
	//    y = tan(θ) x - g * x^2 / (2 (v*cos(θ))^2)
	glBegin(GL_LINE_STRIP);

	float distance = ((frog.r0.speed*frog.r0.speed) / gravity) * sin(2*frog.r0.angle);
	for (int i = 0; i <= segments; i++)
	{
		float x = (i/(float)segments)*distance;
		float y1 = 2 * pow(cos(frog.r0.angle)*frog.r0.speed,2);
		float y = tan(frog.r0.angle) * x - (gravity*x*x)/y1;
		if (debug)
			printf("drawCartesianParabola: d = %.5f\tx = %.5f\ty = %.5f\n", distance, x, y);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, 0.5);

	}
	glEnd();
}
void drawParametricParabola()
{
	//    x = v t cos(θ)
	//    y = v t sin(θ) - 1/2 g t^2
	glBegin(GL_LINE_STRIP);

	float distance = (2*(frog.r0.speed*sin(frog.r0.angle)))/(gravity);
	for (int i = 0; i <= segments; i++)
	{
		float t = (i/(float)segments)*distance;
		float x = frog.r0.speed * t * cos(frog.r0.angle);
		float y = frog.r0.speed * t * sin(frog.r0.angle) - (gravity*t*t)/2;
		if (debug)
			printf("drawParametricParabola: d = %.5f\tx = %.5f\ty = %.5f\n", distance, x, y);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, 0.5);

	}
	glEnd();
}
void drawParabola()
{
	if (parabolaInsideWindow())
		glColor3f(0, 0, 1);
	else
		glColor3f(1, 0, 0);

	if (cartesianFlag)
		drawCartesianParabola();
	else
		drawParametricParabola();
}

void drawParabolaNormalTangent(void)
{
	float x, y, dx, dy, n;
	for (int i = 0; i < segments; i++)
	{
		if (cartesianFlag)
		{
			float distance = calcReach();
			x = (i/(float)segments)*distance;
			float y1 = 2 * pow(cos(frog.r0.angle)*frog.r0.speed,2);
			y = tan(frog.r0.angle) * x - (gravity*x*x)/y1;
			dy = tan(frog.r0.angle) - (gravity*x)/(pow(cos(frog.r0.angle)*frog.r0.speed,2));
			dx = 1;
		}
		else
		{
			float t = (i/(float)segments)*(2*(frog.r0.speed*sin(frog.r0.angle)))/(gravity);
			x = frog.r0.speed * t * cos(frog.r0.angle);
			y = frog.r0.speed * t * sin(frog.r0.angle) - (gravity*t*t)/2;
			dx = frog.r0.speed * cos(frog.r0.angle);
			dy = frog.r0.speed * sin(frog.r0.angle) - gravity*t;
		}

		if(tangentFlag)
		{
			glBegin(GL_LINES);
			glColor3f (0, 1, 1);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, 0);
			n = sqrt(dx*dx + dy*dy)*REDUCTION;
			glVertex3f(frog.r0.x + dx/n + x, frog.r0.y + dy/n + y, 0);

			glEnd();
		}

		if(normalFlag)
		{
			glBegin(GL_LINES);
			glColor3f (1, 1, 0);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, 0);
			n = sqrt(dx*dx + dy*dy)*REDUCTION;
			glVertex3f(frog.r0.x - dy/n + x, frog.r0.y + dx/n + y, 0);

			glEnd();
		}
	}
}

float calcReach(void)
{
	return ((frog.r0.speed*frog.r0.speed) / gravity) * sin(2*frog.r0.angle);
}

bool parabolaInsideWindow(void)
{
	if(debug)
		printf("calcReach:%f speed:%f angle:%f\n",
				calcReach(), frog.r0.speed, frog.r0.angle*180/M_PI);
	if ((calcReach() + frog.r0.x < 1.0) && (calcReach() + frog.r0.x  > -1.0))
		return true;

	return false;
}

void updateFrog(float dt)
{
		float t = (2*(frog.r0.speed*sin(frog.r0.angle)))/(gravity);
		if (dt > t)
			dt = t;

		frog.r.x = frog.r0.speed * dt * cos(frog.r0.angle) + frog.r0.x;
		frog.r.y = frog.r0.speed * dt * sin(frog.r0.angle) - (gravity*dt*dt)/2 + frog.r0.y;

		if (dt == t)
		{
			jumpingFlag = false;
			frog.r0.x = frog.r.x;
			frog.r0.y = frog.r.y;
		}
}

void idle()
{
	float t, dt;
	static float tLast = 0.0;

	if(!jumpingFlag)
		return;

	/* Get elapsed time and convert to s */
	t = glutGet(GLUT_ELAPSED_TIME);
	t /= 1000.0;

	/* Calculate delta t */
	dt = t - tLast;

	/* Update velocity and position */
	updateFrog(t-startTime);
	printf("updateFrog: t=%f\tstartTime=%f\tx=%f\ty=%f\n", t, startTime, frog.r.x, frog.r.y);

	/* Update tLast for next time, using static local variable */
	tLast = t;

	/* Ask glut to schedule call to display function */
	glutPostRedisplay();
}
