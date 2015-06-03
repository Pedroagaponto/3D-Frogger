#ifndef GEOMETRY_H
#define GEOMETRY_H
#include "core.h"

#define CYLINDER_RADIUS 1
#define CYLINDER_HEIGHT 3
#define CUBE_LENGTH 1

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

