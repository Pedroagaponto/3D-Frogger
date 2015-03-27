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
void idle(void);
void calcPosition(float t, float dt);
void calcPositionAnalytical(float t);
void calcPositionNumerical(float dt);

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
bool analyticFlag = true;

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
	if (debug)
		printf("\n\n");
}

void keyboard(unsigned char key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	switch (key)
	{
		case ' ':
			if (!jumpingFlag && parabolaInsideWindow())
			{
				jumpingFlag = true;
				startTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
				frog.r.angle = frog.r0.angle;
				frog.r.speed = frog.r0.speed;
				glutPostRedisplay();
			}
			break;
		case 'f':
		case 'F':
			cartesianFlag = !cartesianFlag;
			glutPostRedisplay();
			break;
		case 'v':
		case 'V':
			debug = !debug;
			break;
		case 't':
		case 'T':
			tangentFlag = !tangentFlag;
			glutPostRedisplay();
			break;
		case 'n':
		case 'N':
			normalFlag = !normalFlag;
			glutPostRedisplay();
			break;
		case 'a':
		case 'A':
			if (jumpingFlag)
				break;
			frog.r0.angle = (frog.r0.angle < M_PI-M_PI/32) ?
				frog.r0.angle+M_PI/32 : M_PI-M_PI/32;
			glutPostRedisplay();
			break;
		case 'd':
		case 'D':
			if (jumpingFlag)
				break;
			frog.r0.angle = (frog.r0.angle < M_PI/16) ?
				M_PI/32 : frog.r0.angle-M_PI/32;
			glutPostRedisplay();
			break;
		case 'w':
		case 'W':
			if (jumpingFlag)
				break;
			frog.r0.speed = (frog.r0.speed > 9.8) ? 10 : frog.r0.speed+0.2;
			glutPostRedisplay();
			break;
		case 's':
		case 'S':
			if (jumpingFlag)
				break;
			frog.r0.speed = (frog.r0.speed < 1.2) ? 1 : frog.r0.speed-0.2;
			glutPostRedisplay();
			break;
		case 'i':
		case 'I':
			if (jumpingFlag)
				break;
			analyticFlag = !analyticFlag;
			break;
		case 27:
		case 'q':
		case 'Q':
			if (debug)
				printf("\n>>>>>EXIT<<<<<\n");
			exit(EXIT_SUCCESS);
		default:
			break;
	}
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
		printf(">>>>>AXES DREW<<<<<\n");
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
	if (debug)
		printf(">>>>PARAMETRIC CIRCLE<<<<<\n");
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
	if (debug)
		printf(">>>>CARTESIAN CIRCLE<<<<<\n");
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
	if (jumpingFlag)
	{
		glVertex3f(frog.r.x, frog.r.y, 0);
		glVertex3f(frog.r.speed*0.1*cos(frog.r.angle)+frog.r.x,
				frog.r.speed*0.1*sin(frog.r.angle)+frog.r.y, 0);
	}
	else
	{
		glVertex3f(frog.r0.x, frog.r0.y, 0);
		glVertex3f(frog.r0.speed*0.1*cos(frog.r0.angle)+frog.r0.x,
				frog.r0.speed*0.1*sin(frog.r0.angle)+frog.r0.y, 0);
	}
	glEnd();
}

void drawCartesianParabola()
{
	//y = tan(θ) x - g * x^2 / (2 (v*cos(θ))^2)
	glBegin(GL_LINE_STRIP);

	float distance = ((frog.r0.speed*frog.r0.speed) / gravity) *
		sin(2*frog.r0.angle);
	for (int i = 0; i <= segments; i++)
	{
		float x = (i/(float)segments)*distance;
		float y1 = 2 * pow(cos(frog.r0.angle)*frog.r0.speed,2);
		float y = tan(frog.r0.angle) * x - (gravity*x*x)/y1;
		if (debug)
			printf("drawCartesianParabola: d = %.5f\tx = %.5f\ty = %.5f\n",
					distance, x + frog.r0.x, y + frog.r0.y);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, 0.5);

	}
	glEnd();
}

void drawParametricParabola()
{
	//x = v t cos(θ)
	//y = v t sin(θ) - 1/2 g t^2
	glBegin(GL_LINE_STRIP);

	float distance = (2*(frog.r0.speed*sin(frog.r0.angle)))/(gravity);
	for (int i = 0; i <= segments; i++)
	{
		float t = (i/(float)segments)*distance;
		float x = frog.r0.speed * t * cos(frog.r0.angle);
		float y = frog.r0.speed * t * sin(frog.r0.angle) - (gravity*t*t)/2;
		if (debug)
			printf("drawParametricParabola: d = %.5f\tx = %.5f\ty = %.5f\n",
					distance, x + frog.r0.x, y + frog.r0.y);
		glVertex3f(frog.r0.x + x, frog.r0.y + y, 0.5);

	}
	glEnd();
}

void drawParabola()
{
	if (debug)
		printf(">>>>>PARABOLA<<<<<\ncalcReach:%f speed:%f angle:%f\n",
				calcReach(), frog.r0.speed, frog.r0.angle*180/M_PI);
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
	for (int i = 0; i <= segments; i++)
	{
		if (cartesianFlag)
		{
			float distance = calcReach();
			x = (i/(float)segments)*distance;
			float y1 = 2 * pow(cos(frog.r0.angle)*frog.r0.speed,2);
			y = tan(frog.r0.angle) * x - (gravity*x*x)/y1;
			dy = tan(frog.r0.angle) -
				(gravity*x)/(pow(cos(frog.r0.angle)*frog.r0.speed,2));
			dx = 1;
		}
		else
		{
			float t = (i/(float)segments)*
				(2*(frog.r0.speed*sin(frog.r0.angle)))/(gravity);
			x = frog.r0.speed * t * cos(frog.r0.angle);
			y = frog.r0.speed * t * sin(frog.r0.angle) - (gravity*t*t)/2;
			dx = frog.r0.speed * cos(frog.r0.angle);
			dy = frog.r0.speed * sin(frog.r0.angle) - gravity*t;
		}

		if (tangentFlag)
		{
			glBegin(GL_LINES);
			glColor3f (0, 1, 1);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, 0);
			n = sqrt(dx*dx + dy*dy)*REDUCTION;
			if (cartesianFlag && x < 0)
				glVertex3f(frog.r0.x - dx/n + x, frog.r0.y - dy/n + y, 0);
			else
				glVertex3f(frog.r0.x + dx/n + x, frog.r0.y + dy/n + y, 0);

			glEnd();
		}

		if (normalFlag)
		{
			glBegin(GL_LINES);
			glColor3f (1, 1, 0);

			glVertex3f(frog.r0.x + x, frog.r0.y + y, 0);
			n = sqrt(dx*dx + dy*dy)*REDUCTION;
			if (!cartesianFlag && x < 0)
				glVertex3f(frog.r0.x + dy/n + x, frog.r0.y - dx/n + y, 0);
			else
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
	if ((calcReach() + frog.r0.x < 1.0) && (calcReach() + frog.r0.x > -1.0))
		return true;

	return false;
}

void idle()
{
	static float tLast = -1.0;
	float t, dt;

	if (!jumpingFlag)
		return;

	t = glutGet(GLUT_ELAPSED_TIME) / 1000.0 - startTime;

	if (tLast < 0.0)
	{
		tLast = t;
		return;
	}

	dt = t - tLast;

	if (debug)
		printf(">>>>>JUMP<<<<<\nt=%f\tstartTime=%f\tx=%f\ty=%f\n",
				t, startTime, frog.r.x, frog.r.y);
	calcPosition(t, dt);

	if (!jumpingFlag)
		tLast = -1;
	else
		tLast = t;

	glutPostRedisplay();
}

void calcPosition(float t, float dt)
{
	if (analyticFlag)
		calcPositionAnalytical(t);
	else
		calcPositionNumerical(dt);
}

void calcPositionAnalytical(float t)
{
	float tEnd = (2*(frog.r0.speed*sin(frog.r0.angle)))/(gravity);
	if (t > tEnd)
		t = tEnd;
	float x = frog.r0.speed * t * cos(frog.r0.angle);
	frog.r.x = frog.r0.x + x;

	if (cartesianFlag)
		frog.r.y = frog.r0.y + tan(frog.r0.angle)*x-(gravity*x*x)/
			(2 * pow(cos(frog.r0.angle)*frog.r0.speed,2));
	else
		frog.r.y = frog.r0.speed * t * sin(frog.r0.angle) -
			(gravity*t*t)/2 + frog.r0.y;

	if (debug)
		printf("Analytical Jump: angle %f, speed %f, x %f, y %f\n",
				frog.r.angle*180/M_PI, frog.r.speed, frog.r.x, frog.r.y);
	if (t == tEnd)
	{
		jumpingFlag = false;
		frog.r0.x = frog.r.x;
		frog.r0.y = frog.r.y;
	}

	float dx = frog.r0.speed * cos(frog.r0.angle);
	float dy = frog.r0.speed * sin(frog.r0.angle) - gravity*t;
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

	speedY += -gravity * dt;
	frog.r.speed = sqrt(speedX*speedX + speedY*speedY);
	frog.r.angle = atan(speedY/speedX);
	if (speedX < 0)
		frog.r.angle+=M_PI;

	if (debug)
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
