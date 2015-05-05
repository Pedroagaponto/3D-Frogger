#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>

#include "procedural.h"
#include "core.h"
#include "geometry.h"

#define RAND(min, max) ((rand()%(int)((max+1)-min)) + min)

vertex randVertex(vertex *array, vertex lenght, int zMin, int zMax, int index);

vertex cars[20];
vertex logs[20];

void initCars(void)
{
	vertex length = {CUBE_LENGTH, CUBE_LENGTH, CUBE_LENGTH};
	for (int i = 0; i < 20; i++)
	{
		cars[i] = randVertex(cars, length, 30, 45, i);
		if (getDebug())
			printf("randCar=(%.0f, %.0f, %.0f)\n",
					cars[i].x, cars[i].y, cars[i].z);
	}
}

void initLogs(void)
{
	vertex length = {CYLINDER_HEIGHT, CYLINDER_RADIUS*2, CYLINDER_RADIUS*2};
	for (int i = 0; i < 20; i++)
	{
		logs[i] = randVertex(logs, length, 2, 20, i); 
		if (getDebug())
			printf("randLog=(%.0f, %.0f, %.0f)\n",
					logs[i].x, logs[i].y, logs[i].z);
	}
}

void drawCars(void)
{
	for (int i = 0; i < 20; i++)
	{
		glTranslatef(cars[i].x, cars[i].y, cars[i].z);
		drawCube();
		glTranslatef(-cars[i].x, -cars[i].y, -cars[i].z);
	}
}

void drawLogs(void)
{
	for (int i = 0; i < 20; i++)
	{
		glTranslatef(logs[i].x, logs[i].y, logs[i].z);
		drawCylinder();
		glTranslatef(-logs[i].x, -logs[i].y, -logs[i].z);
	}
}

vertex randVertex(vertex *array, vertex lenght, int zMin, int zMax, int index)
{
	int i;
	vertex v = {0, 0, 0};

	while (1)
	{
		v.x = RAND(-GRID_WIDTH/2, GRID_WIDTH/2);
		v.z = RAND(zMin, zMax);
		for (i = 0; i < index; i++)
			if ((array[i].x >= v.x) && (array[i].x <= lenght.x + v.x) &&
				(array[i].z >= v.z) && (array[i].z <= lenght.z + v.z))
				break;
		if (i == index)
			break;
	}

	return v;
}

