#include <stdio.h>
#include <string.h>

#include <GL/glut.h>

#include "core.h"

typedef struct {
	float theta, phi, zoom;
} camAttr;

/* extern variables */
frogState frog;
vertex logs[20];

/* initialization variables */
static const frogState frogInit = {
/*	{   x,   y,   z,   r,  theta,    phi,  dx,  dy,  dz}*/
	{ 0.0, 0.0, 48.0, 2.0, M_PI/4, M_PI, 0.0, 0.0, 0.0},
	{ 0.0, 0.0, 48.0, 2.0, M_PI/4, M_PI, 0.0, 0.0, 0.0},
	false
};
static const camAttr camInit = {45, 0, 5};

/* attributes variables*/
static camAttr cam;
static int segments = 10;
static int score = 0;
static int lifes = 5;
static int width = 500;
static int height = 500;
static int oldTime = 0;
static int fps = 0;
static int frameCount = 0;

/* flags */
static bool debug = false;
static bool pause = false;
static bool cartesianFlag = true;
static bool normalFlag = true;
static bool wireFlag = true;
static bool axesFlag = true;
static bool lightFlag = true;
static bool textureFlag = true;
static bool verletFlag = true;

void resetGame(void)
{
	resetPerformance();
	cam = camInit;
	frog = frogInit;
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
	upY = offsetYcamY-camY;
	upZ = offsetYcamZ-camZ;


	glLoadIdentity();
	gluLookAt(frog.r.x + camX, frog.r.y + camY, frog.r.z + camZ,
			  frog.r.x, frog.r.y, frog.r.z,
			  upX, upY, upZ);
}

void drawText(char *text, float x, float y, float z)
{
	int length = (int) strlen(text);
	
	glRasterPos3f(x, y, z);
	for (int i = 0; i < length; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
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

