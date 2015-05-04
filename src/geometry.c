#include <stdio.h>

#include <GL/glut.h>

#include "geometry.h"
#include "core.h"

#define GRID_WIDTH 100
#define GRID_HEIGHT 100
#define S_SLICES 8
#define S_STACKS 8
#define S_RADIUS 0.1
#define C_SLICES 8
#define C_STACKS 2
#define C_RADIUS 0.1
#define C_HEIGHT 1

static vertex vAxes[3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static vertex vOrigin = {0, 0, 0};
static vertex *vGrid = NULL;
static vertex *vSphere = NULL;
static vertex *vCylinder = NULL;

static int *iGrid = NULL;
static int *iSphere = NULL;
static int *iCylinder = NULL;

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
		glNormal3fv((float *) &vGrid[iGrid[i]]);
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
	vSphere = (vertex *) calloc(S_SLICES*(S_STACKS+2), sizeof(vertex));
	iSphere = (int *) calloc(S_SLICES*(S_STACKS+2)*6, sizeof(int));
	if (!vSphere || !iSphere)
	{
		if (getDebug())
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	float theta, phi;
	int iCount = 0;
	vertex *vAux = vSphere;

	for (int j = 0; j <= (S_STACKS+1); j++) {
		phi = j / (float)S_STACKS * M_PI;
		for (int i = 0; i < S_SLICES; i++) {
			theta = i / (float)S_SLICES * 2.0 * M_PI;
			vAux->x = S_RADIUS * sinf(phi) * cosf(theta);
			vAux->y = S_RADIUS * sinf(phi) * sinf(theta);
			vAux->z = S_RADIUS * cosf(phi);
			vAux++;

			if (j == S_SLICES)
				continue;

			iSphere[iCount++] = (i*S_SLICES)+j;
			iSphere[iCount++] = (i*S_SLICES)+j+1;
			iSphere[iCount++] = ((i+1)*S_SLICES)+j;
			iSphere[iCount++] = (i*S_SLICES)+j+1;
			iSphere[iCount++] = ((i+1)*S_SLICES)+j;
			iSphere[iCount++] = ((i+1)*S_SLICES)+j+1;
		}
	}
}

void drawSphere(void)
{
	glTranslatef(frog.r.x, frog.r.y, frog.r.z);
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < S_SLICES*(S_STACKS+2)*6; i++)
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
		printf(">>>>SPHERE DREW<<<<<\n");
}

void drawSphereNormals(void)
{
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	for (int i = 0; i < S_SLICES*(S_STACKS+2); i++)
	{
		glVertex3fv((float *) &vSphere[i]);
		glVertex3f(vSphere[i].x*1.2, vSphere[i].y*1.2, vSphere[i].z*1.2);
	}
	glEnd();
}

void initCylinder(void)
{
	vCylinder = (vertex *) calloc(C_SLICES*C_STACKS, sizeof(vertex));
	iCylinder = (int *) calloc(C_SLICES*C_STACKS*6, sizeof(int));
	if (!vCylinder || !iCylinder)
	{
		if (getDebug())
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	float theta, height;
	int iCount = 0;
	vertex *vAux = vCylinder;

	for (int j = 0; j < C_STACKS; j++) {
		height = j/C_HEIGHT;
		for (int i = 0; i < C_SLICES; i++) {
			theta = i / (float)C_SLICES * 2.0 * M_PI;
			vAux->x = C_RADIUS * cosf(theta);
			vAux->y = C_RADIUS * sinf(theta);
			vAux->z = height;
			vAux++;

			iCylinder[iCount++] = (i*C_SLICES)+j;
			iCylinder[iCount++] = (i*C_SLICES)+j+1;
			iCylinder[iCount++] = ((i+1)*C_SLICES)+j;
			iCylinder[iCount++] = (i*C_SLICES)+j+1;
			iCylinder[iCount++] = ((i+1)*C_SLICES)+j;
			iCylinder[iCount++] = ((i+1)*C_SLICES)+j+1;
		}
	}
}

void drawCylinder(void)
{
	glColor3f(1, 0, 0);
	glBegin(GL_POINTS);
	for (int i = 0; i < C_SLICES*C_STACKS*6; i++)
	{
		glNormal3fv((float *) &vCylinder[iCylinder[i]]);
		glVertex3fv((float *) &vCylinder[iCylinder[i]]);
	}
	glEnd();

	drawAxes(1);

	if (getDebug())
		printf(">>>>CYLINDER DREW<<<<<\n");
}

