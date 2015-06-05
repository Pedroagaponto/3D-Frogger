#include <stdio.h>
#include <string.h>

#include <GL/glut.h>

#include "core.h"

#define MAX_LIFES 5

typedef struct {
	float theta, phi, zoom;
} camAttr;

void settingsGameMode(void);
void settingsDeveloperMode(void);

/* extern variables */
frogState frog;
vertex cars[OBSTACLE_SIZE][LINE_OBSTACLES];
vertex logs[OBSTACLE_SIZE][LINE_OBSTACLES];

/* initialization variables */
static const frogState frogInit = {
/*	{ x,  y,   z,    r,          theta,  phi,      dx,  dy,  dz}*/
	{ 26, 0.0, 50.0, 8.94427191, M_PI/4, 5*M_PI/2, 0.0, 0.0, 0.0},
	{ 26, 0.0, 50.0, 8.94427191, M_PI/4, 5*M_PI/2, 0.0, 0.0, 0.0},
	false,
	0
};
static const camAttr camInit = {45, -90, 5};

/* attributes variables*/
static camAttr cam;
static int segments = 10;
static int score = 0;
static int lifes = MAX_LIFES;
static int width = 1000;
static int height = 1000;
static int oldTime = 0;
static int fps = 0;
static int frameCount = 0;

/* flags */
static bool debug = false;
static bool gameMode = true;
static bool pause = false;
static bool cartesianFlag = true;
static bool normalFlag = true;
static bool wireFlag = true;
static bool axesFlag = true;
static bool lightFlag = true;
static bool textureFlag = true;
static bool verletFlag = true;
static bool gameOver = false;
static bool jumpingFlag = false;

void resetGame(void)
{
	jumpingFlag = false;
	debug = false;
	lightFlag = true;
	verletFlag = true;

	cam = camInit;

	if (gameMode)
		settingsGameMode();
	else
		settingsDeveloperMode();

	resetPerformance();
	frog = frogInit;

	if (gameOver)
	{
		score = 0;
		lifes = MAX_LIFES;
	}
	gameOver = false;
}

void settingsGameMode(void)
{
	glutSetCursor(GLUT_CURSOR_NONE);
	pause = false;
	normalFlag = false;
	wireFlag = false;
	axesFlag = false;
	textureFlag = true;
}

void settingsDeveloperMode(void)
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	pause = true;
	normalFlag = true;
	wireFlag = true;
	axesFlag = true;
	textureFlag = false;
	cam.zoom = 20;
}
void resetPerformance(void)
{
	frameCount = 0;
	fps = 0;
	oldTime = glutGet(GLUT_ELAPSED_TIME);
}

void setProjectionMatrix(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (width / height), 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void setupCamera(void)
{
	float yFactor = 0;
	float camX, camY, camZ;
	float offsetYcamX, offsetYcamY, offsetYcamZ;
	float upX, upY, upZ;
	const float offset = 1.0;

	camX = cam.zoom * sin(cam.theta*M_PI/180) * sin(cam.phi*M_PI/180);
	camZ = cam.zoom * sin(cam.theta*M_PI/180) * cos(cam.phi*M_PI/180);
	camY = cam.zoom * cos(cam.theta*M_PI/180);

	// Changing on the y to get a new up vector
	offsetYcamX = cam.zoom * sin(cam.theta*M_PI/180-offset) *
		sin(cam.phi*M_PI/180);
	offsetYcamZ = cam.zoom * sin(cam.theta*M_PI/180-offset) *
		cos(cam.phi*M_PI/180);
	offsetYcamY = cam.zoom * cos(cam.theta*M_PI/180-offset);

	upX = offsetYcamX-camX;
	upZ = offsetYcamZ-camZ;
	upY = offsetYcamY-camY;

	if (gameMode)
		yFactor = 5;

	glLoadIdentity();
	gluLookAt(frog.r.x + camX, frog.r.y + camY + yFactor, frog.r.z + camZ,
			  frog.r.x, frog.r.y + yFactor, frog.r.z,
			  upX, upY, upZ);
}

void drawText(char *text, float x, float y, float z, void *font)
{
	int length = (int) strlen(text);

	glRasterPos3f(x, y, z);
	for (int i = 0; i < length; i++)
			glutBitmapCharacter(font, text[i]);
}

void calculatePerformance(void)
{
	int time = glutGet(GLUT_ELAPSED_TIME);

	if (time - oldTime >= 1000)
	{
		fps = frameCount;
		frameCount = 0;
		oldTime += 1000;
	}
}

void incFrameCount(void)
{
	frameCount++;
}

void setSegments(int newSegments)
{
	segments = newSegments;
}

void setScore(int newScore)
{
	score = newScore;
}

void setLifes(int newLifes)
{
	lifes = newLifes;
}

void setWidth(int newWidth)
{
	width = newWidth;
}

void setHeight(int newHeight)
{
	height = newHeight;
}

void setRotateCamTheta(float newRotateCamTheta)
{
	cam.theta = (newRotateCamTheta > 0 && newRotateCamTheta < 180)
		         ? newRotateCamTheta : cam.theta;
}

void setRotateCamPhi(float newRotateCamPhi)
{
	cam.phi = newRotateCamPhi;
}

void setCamZoom(float newCamZoom)
{
	cam.zoom = newCamZoom;
}

void updateCartesian(projection *p)
{
	p->dz = p->r * sin(p->theta) * cos(p->phi);
	p->dx = p->r * sin(p->theta) * sin(p->phi);
	p->dy = p->r * cos(p->theta);
}

void updateSpherical(projection *p)
{
	p->r = sqrt(p->dz*p->dz + p->dx*p->dx + p->dy*p->dy);
	p->theta = acos(p->dy / p->r);
	p->phi = atan2( p->dx, p->dz );
	//p->phi += ( p->phi < 0 ) ? 2*M_PI : 0;
}

void switchDebug(void)
{
	debug = !debug;
}

void switchGameMode(void)
{
	gameMode = !gameMode;
}

void switchPause(void)
{
	pause = !pause;
}

void switchCartesianFlag(void)
{
	cartesianFlag = !cartesianFlag;
}

void switchNormalFlag(void)
{
	normalFlag = !normalFlag;
}

void switchWireFlag(void)
{
	wireFlag = !wireFlag;
}

void switchAxesFlag(void)
{
	axesFlag = !axesFlag;
}

void switchLightFlag(void)
{
	lightFlag = !lightFlag;
}

void switchTextureFlag(void)
{
	textureFlag = !textureFlag;
}

void switchVerletFlag(void)
{
	verletFlag = !verletFlag;
}

void setGameOver(bool flag)
{
	gameOver = flag;
	if (gameOver)
		pause = true;
}

void setJumpingFlag(bool flag)
{
	jumpingFlag = flag;
}

int getSegments(void)
{
	return segments;
}

int getScore(void)
{
	return score;
}

int getLifes(void)
{
	return lifes;
}

int getWidth(void)
{
	return width;
}

int getHeight(void)
{
	return height;
}

int getFps(void)
{
	return fps;
}

float getRotateCamTheta(void)
{
	return cam.theta;
}

float getRotateCamPhi(void)
{
	return cam.phi;
}

float getCamZoom(void)
{
	return cam.zoom;
}

bool getDebug(void)
{
	return debug;
}

bool getGameMode(void)
{
	return gameMode;
}

bool getPause(void)
{
	return pause;
}

bool getCartesianFlag(void)
{
	return cartesianFlag;
}

bool getNormalFlag(void)
{
	return normalFlag;
}

bool getWireFlag(void)
{
	return wireFlag;
}

bool getAxesFlag(void)
{
	return axesFlag;
}

bool getLightFlag(void)
{
	return lightFlag;
}

bool getTextureFlag(void)
{
	return textureFlag;
}

bool getVerletFlag(void)
{
	return verletFlag;
}

bool getGameOver(void)
{
	return gameOver;
}

bool getJumpingFlag(void)
{
	return jumpingFlag;
}

