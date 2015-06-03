#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <stdio.h>
#include <GL/gl.h>
#include "core.h"

/* Struct for the mesh, it consider there will be drawn as triangles.
 * Surface = sizei/3
 * size of vn = sizei/ipn */
typedef struct {
	/* Vertex array */
	vertex *v;
	/* Size of vertex array */
	int sizev;
	/* Vertex index array */
	int *vi;
	/* Size of vertex array */
	int sizevi;
	/* Normal Vertex array */
	vertex *vn;
	/* Index per Normal vector, 1 for normal vertex and 3 for surface normal */
	int ipn;
	/* Color array */
	float color[3];
} mesh;


typedef struct vertexInfo
{
	vertex p;
	vertex n;
	vertex2 t;
} vertexInfo;

typedef struct newMesh
{
	// Mesh data is stored in arrays, separated between vertices and indices.
	size_t nVerts;
	vertexInfo *vertices;

	size_t nIndices;
	int *indices;

	GLuint obj;
} newMesh;

void drawAxes(float scale);
void initGrid(void);
void drawGrid(void);
void initSphere(void);
void drawSphere(void);
void initCylinder(void);
void drawCylinder(void);
void initCube(void);
void drawCube(void);

#endif

