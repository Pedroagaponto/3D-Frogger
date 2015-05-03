#include <stdio.h>

#include <GL/glut.h>

#include "core.h"

#define GRID_WIDTH 100
#define GRID_HEIGHT 100

static vertex vAxes[3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

frogState frog = {
	{0.0, 0.0, 0.0, 2.0, M_PI/4},
	{0.0, 0.0, 0.0, 2.0, M_PI/4}
};

static vertex vOrigin = {0, 0, 0};
static vertex *vGrid = NULL;

static int *iGrid = NULL;

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
	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++)
	{
		glColor3fv((float*) &vAxes[i]);
		glVertex3fv((float*) &vOrigin);
		glVertex3fv((float*) &vAxes[i]);
	}
	glEnd();

	if (debug)
		printf(">>>>>AXES DREW<<<<<\n");
}

void initGrid(void)
{
	vGrid = (vertex *) calloc((GRID_WIDTH+1)*(GRID_HEIGHT+1), sizeof(vertex));
	iGrid = (int *) calloc(GRID_WIDTH*GRID_HEIGHT*6, sizeof(int));
	if (!vGrid || !iGrid)
	{
		if (debug)
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	vertex *vAux = vGrid;
	int iCount = 0;

	for (int i = 0; i < 101; i++)
	{
		for (int j =  0; j < 101; j++)
		{
			vAux->x = i - 50;
			vAux->y = 0;
			vAux->z = j - 50;
			vAux++;

			if (i == 100 || j == 100)
				continue;

			iGrid[iCount++] = (i*101)+j;
			iGrid[iCount++] = (i*101)+j+1;
			iGrid[iCount++] = ((i+1)*101)+j;
			iGrid[iCount++] = (i*101)+j+1;
			iGrid[iCount++] = ((i+1)*101)+j;
			iGrid[iCount++] = ((i+1)*101)+j+1;
		}
	}
}

void drawGrid(void)
{
	glColor3f(0, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < GRID_WIDTH*GRID_HEIGHT*6; i++)
	{
		glNormal3fv((float *) &vGrid[iGrid[i]]);
		glVertex3fv((float *) &vGrid[iGrid[i]]);
	}
	glEnd();

	if (normalFlag)
		drawGridNormals();

	if (getDebug())
		printf(">>>>>GRID DREW<<<<<\n");
}

void drawGridNormals(void)
{
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < (GRID_WIDTH+1)*(GRID_HEIGHT+1); i++)
	{
		glVertex3fv((float *) &vGrid[i]);
		glVertex3f(vGrid[i].x, 0.3, vGrid[i].z);
	}
	glEnd();
}

void setProjectionMatrix(void)
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

