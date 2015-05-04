#include <stdio.h>

#include <GL/glut.h>

#include "geometry.h"
#include "core.h"

#define GRID_WIDTH 100
#define GRID_HEIGHT 100
#define SLICES 8
#define STACKS 8
#define RADIUS 0.1

static vertex vAxes[3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static vertex vOrigin = {0, 0, 0};
static vertex *vGrid = NULL;
static vertex *vSphere = NULL;

static int *iGrid = NULL;
static int *iSphere = NULL;

void drawAxes(float scale)
{
	if (!getAxesFlag())
		return;

	glBegin(GL_LINES);
	for (int i = 0; i < 3; i++)
	{
		glColor3fv((float*) &vAxes[i]);
		glVertex3fv((float*) &vOrigin);
		glVertex3f(scale*vAxes[i].x, scale*vAxes[i].y, scale*vAxes[i].z);
	}
	glEnd();

	if (getDebug())
		printf(">>>>>AXES DREW<<<<<\n");
}

void initGrid(void)
{
	vGrid = (vertex *) calloc((GRID_WIDTH+1)*(GRID_HEIGHT+1), sizeof(vertex));
	iGrid = (int *) calloc(GRID_WIDTH*GRID_HEIGHT*6, sizeof(int));
	if (!vGrid || !iGrid)
	{
		if (getDebug())
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
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < GRID_WIDTH*GRID_HEIGHT*6; i++)
	{
		glNormal3fv((float *) &vGrid[i]);
		glVertex3fv((float *) &vGrid[iGrid[i]]);
	}
	glEnd();

	if (getNormalFlag())
		drawGridNormals();

	drawAxes(50);

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

void initSphere(void)
{
	vSphere = (vertex *) calloc(SLICES*(STACKS+2), sizeof(vertex));
	iSphere = (int *) calloc(SLICES*(STACKS+2)*6, sizeof(int));
	if (!vSphere || !iSphere)
	{
		if (getDebug())
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	float theta, phi;
	int iCount = 0;
	vertex *vAux = vSphere;

	for (int j = 0; j <= (STACKS+1); j++) {
		phi = j / (float)STACKS * M_PI;
		for (int i = 0; i < SLICES; i++) {
			theta = i / (float)SLICES * 2.0 * M_PI;
			vAux->x = RADIUS * sinf(phi) * cosf(theta);
			vAux->y = RADIUS * sinf(phi) * sinf(theta);
			vAux->z = RADIUS * cosf(phi);
			vAux++;

			if (j == SLICES)
				continue;

			iSphere[iCount++] = (i*SLICES)+j;
			iSphere[iCount++] = (i*SLICES)+j+1;
			iSphere[iCount++] = ((i+1)*SLICES)+j;
			iSphere[iCount++] = (i*SLICES)+j+1;
			iSphere[iCount++] = ((i+1)*SLICES)+j;
			iSphere[iCount++] = ((i+1)*SLICES)+j+1;
		}
	}
}

void drawSphere(void)
{
	glTranslatef(frog.r.x, frog.r.y, frog.r.z);
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < SLICES*(STACKS+2)*6; i++)
	{
		glNormal3fv((float *) &vSphere[iSphere[i]]);
		glVertex3fv((float *) &vSphere[iSphere[i]]);
	}
	glEnd();

	if (getNormalFlag())
		drawSphereNormals();

	drawAxes(1);
	glTranslatef(-frog.r.x, -frog.r.y, -frog.r.z);

	if (getDebug())
		printf(">>>>SPHERE<<<<<\n");
}

void drawSphereNormals(void)
{
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < SLICES*(STACKS+2); i++)
	{
		glVertex3fv((float *) &vSphere[i]);
		glVertex3f(vSphere[i].x*1.2, vSphere[i].y*1.2, vSphere[i].z*1.2);
	}
	glEnd();
}
