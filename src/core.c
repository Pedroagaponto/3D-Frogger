#include <stdio.h>

#include <GL/glut.h>

#include "core.h"

static int colours[][3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static int axesPosition[][3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

frogState frog = {
	{ 0.0, 0.0, 0.0, 2.0, M_PI/4 },
	{ 0.0, 0.0, 0.0, 2.0, M_PI/4 }
};

static int segments = 10;

static float width = 500;
static float height = 500;
static float rotateCamX = 0;
static float rotateCamY = 0;
static float camZoom = 1;

static bool debug = true;
static bool cartesianFlag = true;
static bool tangentFlag = true;
static bool normalFlag = true;

void drawAxes(void)
{
	for (int i = 0; i < 3; i++)
	{
		glBegin(GL_LINES);
		glColor3f(colours[i][0], colours[i][1], colours[i][2]);
		glVertex3f(frog.r0.x, frog.r0.y, frog.r0.z);
		glVertex3f(axesPosition[i][0]+frog.r0.x,
				axesPosition[i][1]+frog.r0.y, axesPosition[i][2]+frog.r0.z);
		glEnd();
	}

	if (getDebug())
		printf(">>>>>AXES DREW<<<<<\n");
}

void setProjectionMatrix ()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75*camZoom, (width / height), 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
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

void setRotateCamX(float newRotateCamX)
{
	rotateCamX = newRotateCamX;
}

void setRotateCamY(float newRotateCamY)
{
	rotateCamY = newRotateCamY;
}

void setCamZoom(float newCamZoom)
{
	camZoom = newCamZoom;
}

void switchDebug(void)
{
	debug = !debug;
}

void switchCartesianFlag(void)
{
	cartesianFlag = !cartesianFlag;
}

void switchTangentFlag(void)
{
	tangentFlag = !tangentFlag;
}

void switchNormalFlag(void)
{
	normalFlag = !normalFlag;
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

float getRotateCamX(void)
{
	return rotateCamX;
}

float getRotateCamY(void)
{
	return rotateCamY;
}

float getCamZoom(void)
{
	return camZoom;
}

bool getDebug(void)
{
	return debug;
}

bool getCartesianFlag(void)
{
	return cartesianFlag;
}

bool getTangentFlag(void)
{
	return tangentFlag;
}

bool getNormalFlag(void)
{
	return normalFlag;
}

