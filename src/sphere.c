#include <stdio.h>

#include <GL/glut.h>

#include "sphere.h"
#include "core.h"

#define SLICES 8
#define STACKS 8
#define RADIUS 0.1
static vertex *vSphere = NULL;
static int *iSphere = NULL;

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
	glEnable(GL_NORMALIZE);
	glColor3f(1, 1, 1);
	glTranslatef(frog.r.x, frog.r.y, frog.r.z);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < SLICES*(STACKS+2)*6; i++)
	{
		glNormal3fv((float *) &vSphere[iSphere[i]]);
		glVertex3fv((float *) &vSphere[iSphere[i]]);
	}
	glEnd();

	if (getNormalFlag())
		drawSphereNormals();

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
