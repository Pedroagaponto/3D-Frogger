#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glut.h>

#include "procedural.h"
#include "core.h"
#include "geometry.h"

#define RAND(min, max) ((rand()%(int)((max+1)-min)) + min)
#define V_CARS 0.1
#define V_LOGS 0.03

vertex randVertex(vertex *array, float x, float y, int length, int index);

void initCars(void)
{
	int init = GRASS_SIZE*LINE_WIDTH + LINE_WIDTH/2;
	float x, y = (float) CUBE_LENGTH/2;
	for (int i = 0; i < OBSTACLE_SIZE; i++)
	{
		x = (float) (init + LINE_WIDTH * i);
		for (int j = 0; j < LINE_OBSTACLES; j++)
		{
			cars[i][j] = randVertex(cars[i], x, y, CUBE_LENGTH, j);
			if (getDebug())
				printf("randCar=(%f, %f, %f)\n",
						cars[i][j].x, cars[i][j].y, cars[i][j].z);
		}
	}
}

void initLogs(void)
{
	int init;
	init = (GRASS_SIZE+INTERVAL_SIZE+OBSTACLE_SIZE)*LINE_WIDTH+LINE_WIDTH/2;
	float x, y = (float) (-CYLINDER_RADIUS);
	for (int i = 0; i < OBSTACLE_SIZE; i++)
	{
		for (int j = 0; j < LINE_OBSTACLES; j++)
		{
			x = (float) (init + LINE_WIDTH * i);
			logs[i][j] = randVertex(logs[i], x, y, CYLINDER_HEIGHT, i);
			if (getDebug())
				printf("randLog=(%.0f, %.0f, %.0f)\n",
						logs[i][j].x, logs[i][j].y, logs[i][j].z);
		}
	}
}

void drawCars(void)
{
	for (int i = 0; i < OBSTACLE_SIZE; i++)
		for (int j = 0; j < LINE_OBSTACLES; j++)
		{
			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(cars[i][j].x, cars[i][j].y, cars[i][j].z);
			drawCube();
			glPopMatrix();
		}
}

void drawLogs(void)
{
	for (int i = 0; i < OBSTACLE_SIZE; i++)
		for (int j = 0; j < LINE_OBSTACLES; j++)
		{
			glPushMatrix();
			glColor3f(1, 0, 0);
			glTranslatef(logs[i][j].x, logs[i][j].y, logs[i][j].z);
			glRotatef(90, 0, 1, 0);
			drawCylinder();
			glPopMatrix();
		}
}

void updateCars(void)
{
	float distance = (GRID_WIDTH*LINE_WIDTH)-CUBE_LENGTH;
	for (int i = 0; i < OBSTACLE_SIZE; i++)
		for (int j = 0; j < LINE_OBSTACLES; j++)
	{
		if (i % 2 == 0)
			cars[i][j].z = (cars[i][j].z + V_CARS < distance) ?
				cars[i][j].z + V_CARS : CUBE_LENGTH/2;
		else
			cars[i][j].z = (cars[i][j].z - V_CARS > CUBE_LENGTH) ?
				cars[i][j].z - V_CARS : distance;
	}
}

void updateLogs(void)
{
	float distance = (GRID_WIDTH*LINE_WIDTH)-CYLINDER_HEIGHT;
	for (int i = 0; i < OBSTACLE_SIZE; i++)
		for (int j = 0; j < LINE_OBSTACLES; j++)
		{
			if (i % 2 == 0)
				logs[i][j].z = (logs[i][j].z + V_LOGS < distance) ?
					logs[i][j].z + V_LOGS : CYLINDER_HEIGHT/2;
			else
				logs[i][j].z = (logs[i][j].z - V_LOGS > CYLINDER_HEIGHT) ?
					logs[i][j].z - V_LOGS : distance;
		}

	if (frog.onLog)
	{
		if (frog.r.z + V_LOGS < GRID_WIDTH - CYLINDER_HEIGHT/2)
			frog.r.z = frog.r0.z = frog.r.z + V_LOGS;
		else
			resetGame();
	}
}

vertex randVertex(vertex *array, float x, float y, int length, int index)
{
	int i;
	vertex v = {x, y, 0.0};

	while (1)
	{
		v.z = RAND(length/2, (GRID_WIDTH * LINE_WIDTH) - length/2);
		for (i = 0; i < index; i++)
		{
			if ((array[i].z > v.z - 2*length) && (array[i].z < v.z + 2*length))
				break;
		}
		if (i == index)
			break;
	}

	return v;
}

