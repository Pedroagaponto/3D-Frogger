#include <stdio.h>
#include <GL/glut.h>
#include <SOIL.h>

#include "geometry.h"

#define REDUCE 1
#define S_SLICES 8
#define S_STACKS 8
#define CYLINDER_SLICES 8
#define CUBE_SLICES 4
#define CUBE_RADIUS (sqrt(2)/2)*CUBE_LENGTH
#define WHITE {1.0, 1.0, 1.0}
#define RED   {1.0, 0.0, 0.0}
#define GREEN {0.0, 1.0, 0.0}
#define BLUE  {0.0, 0.0, 1.0}
#define BLACK {0.0, 0.0, 0.0}

void initPrism(int slices, float radius, mesh *m, float height);
void drawGeometry(mesh m);
vertex vertexAddReduce(vertex v1, vertex v2);
vertex vertexAdd(vertex v1, vertex v2);
vertex vertexSub(vertex v1, vertex v2);
vertex vertexCrossProduct(vertex v1, vertex v2);
void normalize(vertex *v);
vertex calcSurfaceNormal(vertex v1, vertex v2, vertex v3);
void initNormals(mesh *m);
void drawNormal(mesh m);
vertex average3(vertex v1, vertex v2, vertex v3);

static vertex vAxes[3] = { RED, GREEN, BLUE };

static vertex vOrigin = {0, 0, 0};
static mesh cylinder = {NULL, 0, NULL, 0, NULL, 3, NULL, -1, BLACK};
static mesh cube     = {NULL, 0, NULL, 0, NULL, 3, NULL, -1, RED};
static mesh sphere   = {NULL, 0, NULL, 0, NULL, 1, NULL, -1, WHITE};
static mesh grid     = {NULL, 0, NULL, 0, NULL, 3, NULL, -1, GREEN};

GLuint loadTexture(const char *filename)
{
	GLuint tex = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (!tex)
		return 0;

	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

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
	grid.sizevi = GRID_WIDTH*GRID_HEIGHT*6;
	grid.v = (vertex *) calloc((GRID_WIDTH+1)*(GRID_HEIGHT+1), sizeof(vertex));
	grid.vi = (int *) calloc(grid.sizevi, sizeof(int));
	grid.ipn = 3;
	if (!grid.v || !grid.vi)
	{
		if (getDebug())
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	vertex *vAux = grid.v;
	int iCount = 0;
	float riverInit = GRASS_SIZE + OBSTACLE_SIZE + INTERVAL_SIZE;

	for (int i = 0; i <= GRID_WIDTH; i++)
	{
		for (int j =  0; j <= GRID_HEIGHT; j++)
		{
			vAux->x = i * LINE_WIDTH;
			vAux->z = j * LINE_WIDTH;
			if ((i <= riverInit) || (i >= riverInit + OBSTACLE_SIZE))
				vAux->y = 0;
			else
				vAux->y = -(CYLINDER_RADIUS * 3);
			vAux++;

			if (i == GRID_WIDTH || j == GRID_HEIGHT)
				continue;

			grid.vi[iCount++] = (i*(GRID_WIDTH+1))+j;
			grid.vi[iCount++] = (i*(GRID_WIDTH+1))+j+1;
			grid.vi[iCount++] = ((i+1)*(GRID_WIDTH+1))+j;
			grid.vi[iCount++] = (i*(GRID_WIDTH+1))+j+1;
			grid.vi[iCount++] = ((i+1)*(GRID_WIDTH+1))+j+1;
			grid.vi[iCount++] = ((i+1)*(GRID_WIDTH+1))+j;
		}
	}

	initNormals(&grid);
}

void drawGrid(void)
{
	drawGeometry(grid);
	drawAxes(50);
}

void initSphere(void)
{
	sphere.sizevi = S_SLICES*S_STACKS*6;
	sphere.vi = (int *) calloc(sphere.sizevi, sizeof(int));
	sphere.sizev = S_SLICES*(S_STACKS+1);
	sphere.v = (vertex *) calloc(sphere.sizev, sizeof(vertex));
	sphere.ipn = 1;
	sphere.vn = (vertex *) calloc(sphere.sizev, sizeof(vertex));
	if (!sphere.v || !sphere.vi)
	{
		if (getDebug())
			printf("ERROR: Out of memory\n");
		exit(1);
	}

	float theta, phi;
	int iCount = 0;
	vertex *vAux = sphere.v;

	for (int j = 0; j < (S_STACKS+1); j++) {
		theta = j / (float)S_STACKS * M_PI;
		for (int i = 0; i < S_SLICES; i++) {
			phi = i / (float)S_SLICES * 2.0 * M_PI;
			vAux->x = S_RADIUS * sinf(theta) * cosf(phi);
			vAux->y = S_RADIUS * sinf(theta) * sinf(phi);
			vAux->z = S_RADIUS * cosf(theta);
			vAux++;

			if (j == S_STACKS)
				continue;

			if(j != 0)
			{
				sphere.vi[iCount++] = (j*S_SLICES)+i;
				sphere.vi[iCount++] = ((j+1)*S_SLICES)+i;
				sphere.vi[iCount++] = (j*S_SLICES)+(i+1)%S_SLICES;
			}
			if(j != S_STACKS - 1)
			{
				sphere.vi[iCount++] = (j*S_SLICES)+(i+1)%S_SLICES;
				sphere.vi[iCount++] = ((j+1)*S_SLICES)+i;
				sphere.vi[iCount++] = ((j+1)*S_SLICES)+(i+1)%S_SLICES;
			}
		}
	}

	for (int i = 0; i < sphere.sizev; i++)
	{
		sphere.vn[i] = sphere.v[i];
		normalize(&sphere.vn[i]);
	}
}

void drawSphere(void)
{
	drawGeometry(sphere);
	drawAxes(1);
}

void initCylinder(void)
{
	cylinder.tex = loadTexture("res/wood.jpg");
	printf ("\n\n%d\n\n", cylinder.tex);
	initPrism(CYLINDER_SLICES, CYLINDER_RADIUS, &cylinder, CYLINDER_HEIGHT);
}

void drawCylinder(void)
{
	glRotatef(90,0,1,0);
	drawGeometry(cylinder);
	glRotatef(-90,0,1,0);
	drawAxes(1);
}

void initCube(void)
{
	initPrism(CUBE_SLICES, CUBE_RADIUS, &cube, CUBE_LENGTH);
}

void drawCube(void)
{
	drawGeometry(cube);
	drawAxes(1);
}

void initPrism(int slices, float radius, mesh *m, float height)
{
	m->sizev = slices*2;
	m->v = (vertex *) calloc(m->sizev, sizeof(vertex));
	m->sizevi = slices*6+(slices-2)*2*3;
	m->vi = (int *) calloc(m->sizevi, sizeof(int));
	m->ipn = 3;
	if (!(m->v) || !(m->vi))
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
			m->v[j*slices + i].x = radius * cosf(theta+M_PI/4);
			m->v[j*slices + i].y = radius * sinf(theta+M_PI/4);
			m->v[j*slices + i].z = height * j - height/2;
		}
	}

	/* Get the vertex index for body */
	for (int i = 0; i < slices; i++)
	{
		m->vi[iCount++] = i;
		m->vi[iCount++] = slices+(i+1)%slices;
		m->vi[iCount++] = slices+i;

		m->vi[iCount++] = i;
		m->vi[iCount++] = (i+1)%slices;
		m->vi[iCount++] = slices+(i+1)%slices;
	}

	/* Get the vertex index for top and bottom */
	for (int i = 1; i < slices - 1; i++)
	{
		m->vi[iCount++] = 0;
		m->vi[iCount++] = i+1;
		m->vi[iCount++] = i;
	}
	for (int i = 1; i < slices - 1; i++)
	{
		m->vi[iCount++] = slices + 0;
		m->vi[iCount++] = slices + i;
		m->vi[iCount++] = slices + i+1;
	}

	initNormals(m);
}

/* Draw the triangles and set normals
 * if ipn==1 there are normal for each vertex, and vn is organized as v
 * if ipn==3 there are normal for each surface, and there is 1 vn to 3 v */
void drawGeometry(mesh m)
{
	glColor3fv(m.color);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < m.sizevi; i++)
	{
		if (m.ipn == 1)
			glNormal3fv((float *) &m.vn[m.vi[i]]);
		else if (m.ipn == 3 && i % 3 == 0)
			glNormal3fv((float *) &m.vn[i/3]);

		glVertex3fv((float *) &m.v[m.vi[i]]);
	}
	glEnd();

	if(getNormalFlag())
		drawNormal(m);
}

/* Draw all normals using the mesh */
void drawNormal(mesh m)
{
	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	if (m.ipn == 3)
	{
		for (int i = 0; i < m.sizevi/3; i++)
		{
			vertex ini = average3(m.v[m.vi[i*3]], m.v[m.vi[i*3+1]],
									m.v[m.vi[i*3+2]]);
			glVertex3fv((float *) &ini);
			vertex end = vertexAddReduce(ini, m.vn[i]);
			glVertex3fv((float *) &end);
		}
	}
	else if (m.ipn == 1)
	{
		for (int i = 0; i < m.sizev; i++)
		{
			glVertex3fv((float *) &m.v[i]);
			vertex end = vertexAddReduce(m.v[i], m.vn[i]);
			glVertex3fv((float *) &end);
		}
	}
	glEnd();
}

vertex vertexAddReduce(vertex v1, vertex v2)
{
	vertex v = {v1.x + v2.x * REDUCE,
				v1.y + v2.y * REDUCE,
				v1.z + v2.z * REDUCE};
	return v;
}

vertex vertexAdd(vertex v1, vertex v2)
{
	vertex v = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return v;
}

vertex vertexSub(vertex v1, vertex v2)
{
	vertex v = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
	return v;
}

vertex vertexCrossProduct(vertex v1, vertex v2)
{
	vertex v = {v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x};
	return v;
}

void normalize(vertex *v)
{
	float length = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
	v->x /= length;
	v->y /= length;
	v->z /= length;
}

/* Calculate the normal using the cross product of the surface */
vertex calcSurfaceNormal(vertex v1, vertex v2, vertex v3)
{
	vertex tmp1 = vertexSub(v2, v1);
	vertex tmp2 = vertexSub(v3, v1);

	vertex v = vertexCrossProduct(tmp1, tmp2);

	normalize(&v);

	return v;
}

/* Allocate m->vn,
 * And fill it using calcSurfaceNormal for all triangle surfaces */
void initNormals(mesh *m)
{
	m->vn = (vertex *) calloc(m->sizevi/3, sizeof(vertex));
	for (int i = 0; i < m->sizevi/3; i++)
	{
		m->vn[i] = calcSurfaceNormal(m->v[m->vi[i*3]], m->v[m->vi[i*3+1]],
									 m->v[m->vi[i*3+2]]);
	}
}

vertex average3(vertex v1, vertex v2, vertex v3)
{
	vertex v = {(v1.x + v2.x + v3.x)/3,
				(v1.y + v2.y + v3.y)/3,
				(v1.z + v2.z + v3.z)/3};

	return v;
}

