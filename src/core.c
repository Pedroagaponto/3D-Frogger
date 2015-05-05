#include <stdio.h>

#include <GL/glut.h>

#include "core.h"

frogState frog = {
/*	{   x,   y,   z,   r,  theta,    phi,  dx,  dy,  dz}*/
	{ 0.0, 0.0, 0.0, 2.0, M_PI/4, M_PI/2, 0.0, 0.0, 0.0},
	{ 0.0, 0.0, 0.0, 2.0, M_PI/4, M_PI/2, 0.0, 0.0, 0.0}
};

static int segments = 10;

static float width = 500;
static float height = 500;
static float rotateCamTheta = 45;
static float rotateCamPhi = 0;
static float camZoom = 5;

static bool debug = false;
static bool pause = false;
static bool cartesianFlag = true;
static bool normalFlag = true;
static bool wireFlag = true;
static bool axesFlag = true;
static bool lightFlag = true;
static bool textureFlag = true;
static bool verletFlag = true;

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

	camX = camZoom * sin(rotateCamTheta*M_PI/180) * sin(rotateCamPhi*M_PI/180);
	camZ = camZoom * sin(rotateCamTheta*M_PI/180) * cos(rotateCamPhi*M_PI/180);
	camY = camZoom * cos(rotateCamTheta*M_PI/180);

	// Changing on the y to get a new up vector
	offsetYcamX = camZoom * sin(rotateCamTheta*M_PI/180-offset) *
		sin(rotateCamPhi*M_PI/180);
	offsetYcamZ = camZoom * sin(rotateCamTheta*M_PI/180-offset) *
		cos(rotateCamPhi*M_PI/180);
	offsetYcamY = camZoom * cos(rotateCamTheta*M_PI/180-offset);

	upX = offsetYcamX-camX;
	upY = offsetYcamY-camY;
	upZ = offsetYcamZ-camZ;


	glLoadIdentity();
	gluLookAt(frog.r.x + camX, frog.r.y + camY, frog.r.z + camZ,
			  frog.r.x, frog.r.y, frog.r.z,
			  upX, upY, upZ);
}

void setSegments(int newSegments)
{
	segments = newSegments;
}

void setWidth(float newWidth)
{
	width = newWidth;
}

void setHeight(float newHeight)
{
	height = newHeight;
}

void setRotateCamTheta(float newRotateCamTheta)
{
	rotateCamTheta = (newRotateCamTheta > 0 && newRotateCamTheta < 180)
		         ? newRotateCamTheta : rotateCamTheta;
}

void setRotateCamPhi(float newRotateCamPhi)
{
	rotateCamPhi = newRotateCamPhi;
}

void setCamZoom(float newCamZoom)
{
	camZoom = newCamZoom;
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

float getWidth(void)
{
	return width;
}

float getHeight(void)
{
	return height;
}

float getRotateCamTheta(void)
{
	return rotateCamTheta;
}

float getRotateCamPhi(void)
{
	return rotateCamPhi;
}

float getCamZoom(void)
{
	return camZoom;
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

