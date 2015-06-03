#include "new_geometry.h"

void render_mesh (newMesh *m)
{
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(vertexInfo), m->vertices);
	glNormalPointer(GL_FLOAT, sizeof(vertexInfo), &m->vertices[0].n);
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertexInfo), &m->vertices[0].t);

	glDrawElements(GL_TRIANGLES, m->nIndices, GL_UNSIGNED_INT, m->indices);

	glPopClientAttrib();

}

