#include <stdio.h>

#include <GL/glut.h>

#include "input.h"
#include "core.h"
#include "jumping_logic.h"

static int oldX;
static int oldY;

static bool leftClickFlag = true;

void gameKeyboard(unsigned char key);
void debugKeyboard(unsigned char key);
void gameSpecialKeys(unsigned char key);
void debugSpecialKeys(unsigned char key);
void keyJump(float phi);

void keyboard(unsigned char key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	switch (key)
	{
		case 'b':
		case 'B':
			if (!getGameOver())
			{
				switchPause();
				glutPostRedisplay();
			}
			break;
		case 27:
		case 'q':
		case 'Q':
			if (getDebug())
				printf("\n>>>>>EXIT<<<<<\n");
			exit(EXIT_SUCCESS);
		case 'm':
		case 'M':
			switchGameMode();
			resetGame();
			break;
		case ' ':
			if (getGameOver())
			{
				oldX = x;
				oldY = y;
				resetGame();
				break;
			}
		default:
			if (getGameMode())
				gameKeyboard(key);
			else
				debugKeyboard(key);
			break;
	}
}

void gameKeyboard(unsigned char key)
{
	switch (key)
	{
		case 'd':
		case 'D':
			keyJump(2 * M_PI);
			break;
		case 'a':
		case 'A':
			keyJump(M_PI);
			break;
		case 'w':
		case 'W':
			keyJump((5 * M_PI) / 2);
			break;
		case 's':
		case 'S':
			keyJump((3 * M_PI) / 2);
			break;
		default:
			break;
	}
}

void debugKeyboard(unsigned char key)
{
	switch (key)
	{
		case ' ':
			jumpingSettings();
			break;
		case 'v':
		case 'V':
			switchDebug();
			break;
		case 't':
		case 'T':
			switchTextureFlag();
			glutPostRedisplay();
			break;
		case 'p':
		case 'P':
			switchWireFlag();
			glutPostRedisplay();
			break;
		case 'o':
		case 'O':
			switchAxesFlag();
			glutPostRedisplay();
			break;
		case 'n':
		case 'N':
			switchNormalFlag();
			glutPostRedisplay();
			break;
		case 'l':
		case 'L':
			switchLightFlag();
			glutPostRedisplay();
			break;
		case 'j':
		case 'J':
			if (!getJumpingFlag())
				switchVerletFlag();
			break;
		case 'd':
		case 'D':
			if (!getJumpingFlag())
			{
				frog.r0.theta = (frog.r0.theta > M_PI/2 - M_PI/16) ?
					M_PI/2-M_PI/32 : frog.r0.theta+M_PI/32;
				updateCartesian(&frog.r0);
				glutPostRedisplay();
			}
			break;
		case 'a':
		case 'A':
			if (!getJumpingFlag())
			{
				frog.r0.theta = (frog.r0.theta < M_PI/16) ?
					M_PI/32 : frog.r0.theta-M_PI/32;
				updateCartesian(&frog.r0);
				glutPostRedisplay();
			}
			break;
		case 'w':
		case 'W':
			if (!getJumpingFlag())
			{
				frog.r0.r = (frog.r0.r > 99.8) ? 100 : frog.r0.r+0.2;
				updateCartesian(&frog.r0);
				glutPostRedisplay();
			}
			break;
		case 's':
		case 'S':
			if (!getJumpingFlag())
			{
				frog.r0.r = (frog.r0.r < 1.2) ? 1 : frog.r0.r-0.2;
				updateCartesian(&frog.r0);
				glutPostRedisplay();
			}
			break;
		default:
			break;
	}
}

void specialKeys(int key, int x, int y)
{
	UNUSED_VAR x;
	UNUSED_VAR y;

	if (!getPause())
	{
		if (getGameMode())
			gameSpecialKeys(key);
		else
			debugSpecialKeys(key);
	}
}

void gameSpecialKeys(unsigned char key)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			keyJump((5 * M_PI) / 2);
			break;
		case GLUT_KEY_DOWN:
			keyJump((3 * M_PI) / 2);
			break;
		case GLUT_KEY_LEFT:
			keyJump(M_PI);
			break;
		case GLUT_KEY_RIGHT:
			keyJump(2 * M_PI);
			break;
		default:
			break;
	}
}

void debugSpecialKeys(unsigned char key)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			setSegments(getSegments()*2);
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			setSegments((getSegments() < 8) ? 4 : getSegments()/2);
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			frog.r0.phi += M_PI/16;
			updateCartesian(&frog.r0);
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			frog.r0.phi -= M_PI/16;
			glutPostRedisplay();
			updateCartesian(&frog.r0);
			break;
		default:
			break;
	}
}

void mouseClick(int button, int state, int x, int y)
{
	UNUSED_VAR state;

	if (!getGameMode())
	{
		oldX = x;
		oldY = y;
		if (button == GLUT_LEFT_BUTTON)
			leftClickFlag = true;
		else if (button == GLUT_RIGHT_BUTTON)
			leftClickFlag = false;
	}
}

void mouseMove(int x, int y)
{
	int diffX = oldX - x, diffY = oldY - y;

	if (!getGameMode())
	{
		if (leftClickFlag)
		{
			setRotateCamPhi(getRotateCamPhi() + diffX);
			setRotateCamTheta(getRotateCamTheta() + diffY);
			glutPostRedisplay();
		}
		else
		{
			setCamZoom(getCamZoom()-diffY*0.01);
			glutPostRedisplay();
		}
		oldX = x;
		oldY = y;
	}
	else if (!getPause())
		setRotateCamPhi(getRotateCamPhi() + (getWidth()/2 - x)*0.1);
}

void passiveMouseMove(int x, int y)
{
	UNUSED_VAR y;

	if ((getGameMode()) && (!getPause()))
		setRotateCamPhi(getRotateCamPhi() + ((getWidth()/2 - x)*0.1));
}

void keyJump(float phi)
{
	if ((!getJumpingFlag()) && (!getPause()))
	{
		frog.r0.phi = phi;
		updateCartesian(&frog.r0);
		jumpingSettings();
	}
}

