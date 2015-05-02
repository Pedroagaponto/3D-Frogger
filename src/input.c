#include <stdio.h>

#include <GL/glut.h>

#include "input.h"
#include "core.h"
#include "jumping_logic.h"

static int oldX = 0;
static int oldY = 0;

static bool leftClickFlag = true;

void keyboard(unsigned char key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	switch (key)
	{
		case ' ':
			jumpingSettings();
			break;
		case 'f':
		case 'F':
			switchCartesianFlag();
			glutPostRedisplay();
			break;
		case 'v':
		case 'V':
			switchDebug();
			break;
		case 't':
		case 'T':
			switchTangentFlag();
			glutPostRedisplay();
			break;
		case 'n':
		case 'N':
			switchNormalFlag();
			glutPostRedisplay();
			break;
		case 'a':
		case 'A':
			if (!getJumpingFlag())
			{
				frog.r0.angle = (frog.r0.angle < M_PI-M_PI/32) ?
					frog.r0.angle+M_PI/32 : M_PI-M_PI/32;
				glutPostRedisplay();
			}
			break;
		case 'd':
		case 'D':
			if (!getJumpingFlag())
			{
				frog.r0.angle = (frog.r0.angle < M_PI/16) ?
					M_PI/32 : frog.r0.angle-M_PI/32;
				glutPostRedisplay();
			}
			break;
		case 'w':
		case 'W':
			if (!getJumpingFlag())
			{
				frog.r0.speed = (frog.r0.speed > 9.8) ? 10 : frog.r0.speed+0.2;
				glutPostRedisplay();
			}
			break;
		case 's':
		case 'S':
			if (!getJumpingFlag())
			{
				frog.r0.speed = (frog.r0.speed < 1.2) ? 1 : frog.r0.speed-0.2;
				glutPostRedisplay();
			}
			break;
		case 'i':
		case 'I':
			if (!getJumpingFlag())
				switchAnalyticFlag();
			break;
		case 27:
		case 'q':
		case 'Q':
			if (getDebug())
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
			setSegments(getSegments()*2);
			break;
		case GLUT_KEY_DOWN:
			setSegments((getSegments() < 8) ? 4 : getSegments()/2);
			break;
		case GLUT_KEY_LEFT:
			//TODO rotate speed component around Y
			break;
		case GLUT_KEY_RIGHT:
			//TODO rotate speed component around Y
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
	UNUSED_VAR state;

	oldX = x;
	oldY = y;
	if (button == GLUT_LEFT_BUTTON)
		leftClickFlag = true;
	else if (button == GLUT_RIGHT_BUTTON)
		leftClickFlag = false;
}

void mouseMove(int x, int y)
{
	int diffX = oldX - x, diffY = oldY - y;

	if (leftClickFlag)
	{
		setRotateCamY(getRotateCamY() + diffX);
		setRotateCamX(getRotateCamX() + diffY);
	}
	else if ((getCamZoom()-(diffY*0.1) > 0.4) &&
			(getCamZoom()-(diffY*0.1) < 1.6))
		setCamZoom(getCamZoom()-diffY*0.01);

	oldX = x;
	oldY = y;
	glutPostRedisplay();
}

