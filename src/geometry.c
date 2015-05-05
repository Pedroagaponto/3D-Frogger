#include <stdio.h>

#include <GL/glut.h>

#include "geometry.h"
#include "core.h"

#define GRID_HEIGHT 100
#define S_SLICES 8
#define S_STACKS 8
#define S_RADIUS 0.1
#define CYLINDER_SLICES 8
#define CUBE_SLICES 4
#define CUBE_RADIUS sqrt(2)/2

void initPrism(int height, int slices, float radius, vertex **v, int **index);
void drawGeometry(int length, vertex *v, int *index, int n, float axesScale);

static vertex vAxes[3] = {
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
};

static vertex vOrigin = {0, 0, 0};
static vertex *vGrid = NULL;
static vertex *vSphere = NULL;
static vertex *vCylinder = NULL;
static vertex *vCube = NULL;

static int *iGrid = NULL;
static int *iSphere = NULL;
static int *iCylinder = NULL;
static int *iCube = NULL;

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

	for (int i = 0; i <= GRID_WIDTH; i++)
	{
		for (int j =  0; j <= GRID_HEIGHT; j++)
		{
			vAux->x = i - GRID_WIDTH/2;
			vAux->y = 0;
			vAux->z = j - GRID_HEIGHT/2;
			vAux++;

			if (i == GRID_WIDTH || j == GRID_HEIGHT)
				continue;

			iGrid[iCount++] = (i*(GRID_WIDTH+1))+j;
			iGrid[iCount++] = (i*(GRID_WIDTH+1))+j+1;
			iGrid[iCount++] = ((i+1)*(GRID_WIDTH+1))+j;
			iGrid[iCount++] = (i*(GRID_WIDTH+1))+j+1;
			iGrid[iCount++] = ((i+1)*(GRID_WIDTH+1))+j;
			iGrid[iCount++] = ((i+1)*(GRID_WIDTH+1))+j+1;
		}
	}
}

void drawGrid(void)
{
	glColor3f(0, 1, 0);

	glBegin(GL_TRIANGLES);
	glNormal3f(0,1,0);
	for (int i = 0; i < GRID_WIDTH*GRID_HEIGHT*6; i++)
		glVertex3fv((float *) &vGrid[iGrid[i]]);
	glEnd();

	if (getNormalFlag())
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
	drawGeometry(S_SLICES*(S_STACKS+2)*6, vSphere, iSphere, 1, 1);
	if (getNormalFlag())
		drawSphereNormals();
	glTranslatef(-frog.r.x, -frog.r.y, -frog.r.z);
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
	initPrism(CYLINDER_HEIGHT, CYLINDER_SLICES, CYLINDER_RADIUS,
			&vCylinder, &iCylinder);
}

void drawCylinder(void)
{
	float length = CYLINDER_SLICES*6+(CYLINDER_SLICES-2)*6;
	drawGeometry(length, vCylinder, iCylinder, 3, 1);
}

void initCube(void)
{
	initPrism(CUBE_LENGTH, CUBE_SLICES, CUBE_RADIUS, &vCube, &iCube);
}

void drawCube(void)
{
	glColor3f(1, 0, 0);
	float length = CUBE_SLICES*6+(CUBE_SLICES-2)*6;
	drawGeometry(length, vCube, iCube, 3, 1);
}

void initPrism(int height, int slices, float radius, vertex **v, int **index)
{
	*v = (vertex *) calloc(slices*2, sizeof(vertex));
	int nIndex = slices*6+(slices-2)*2*3;
	*index = (int *) calloc(nIndex, sizeof(int));
	if (!(*v) || !(*index))
	{
		if (getDebug())
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	float theta;
	int iCount = 0;

	/* Get the vertex */
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < slices; i++)
		{
			theta = i / (float)slices * 2.0 * M_PI;
			(*v)[j*slices + i].z = radius * cosf(theta+M_PI/4);
			(*v)[j*slices + i].y = radius * sinf(theta+M_PI/4);
			(*v)[j*slices + i].x = height * j - height/2;
		}
	}

	/* Get the vertex index for body */
	for (int i = 0; i < slices; i++)
	{
		(*index)[iCount++] = i;
		(*index)[iCount++] = slices+i;
		(*index)[iCount++] = slices+(i+1)%slices;

		(*index)[iCount++] = i;
		(*index)[iCount++] = slices+(i+1)%slices;
		(*index)[iCount++] = (i+1)%slices;
	}

	/* Get the vertex index for top and bottom */
	for (int j = 0; j < 2; j++)
	{
		for (int i = 1; i < slices - 1; i++)
		{
			(*index)[iCount++] = j*slices + 0;
			(*index)[iCount++] = j*slices + i;
			(*index)[iCount++] = j*slices + i+1;
		}
	}
}

void drawGeometry(int length, vertex *v, int *index, int n, float axesScale)
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < length; i++)
	{
		if (i % n == 0)
			glNormal3fv((float *) &v[index[i]]);
		glVertex3fv((float *) &v[index[i]]);
	}
	glEnd();

	drawAxes(axesScale);
}

