#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "objloader.h"

typedef struct
{
	int v, vt, vn;
} index3;

typedef struct{
	int n_v, n_vt, n_vn, n_f;
	vertex *v, *vn;
	vertex2 *vt;
	index3 *f;
}parseResult;

void scanFaceLine(FILE *f, index3 *i3)
{
	int matched, offset;
	static int mode = 0;

	if (mode == 0)
	{
		offset = ftell(f);
		matched = fscanf(f, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&i3[0].v, &i3[0].vt, &i3[0].vn,
				&i3[1].v, &i3[1].vt, &i3[1].vn,
				&i3[2].v, &i3[2].vt, &i3[2].vn);
		if (matched != 9)
		{
			fseek(f, offset, SEEK_SET);
			mode = 1;
		}
	}

	if (mode == 1)
	{
		matched = fscanf(f, "%d//%d %d//%d %d//%d\n",
				&i3[0].v, &i3[0].vn,
				&i3[1].v, &i3[1].vn,
				&i3[2].v, &i3[2].vn);
		i3[0].vt = i3[1].vt = i3[2].vt = 0;
		if (matched != 6)
		{
			mode = 1;
			printf("Failed parsing obj file\n");
			exit (1);
		}
	}
}

int parseObjFile(FILE *objFile, parseResult *result)
{
	char header[128];
	void *new_prt;
	int i, matched;

	result->n_v = result->n_vt = result->n_vn = result->n_f = 0;
	result->v = result->vn = NULL;
	result->vt = NULL;
	result->f = NULL;

	while (fscanf(objFile, "%s", header) != EOF)
	{
		if(!strcmp(header, "v"))
		{
			i = result->n_v;
			result->n_v++;
			new_prt = realloc (result->v, sizeof(vertex) * result->n_v);
			if (new_prt != NULL)
				result->v = (vertex *)new_prt;
			else
			{
				perror("realloc");
				exit(1);
			}
			matched = fscanf(objFile, "%f %f %f\n", &result->v[i].x, &result->v[i].y, &result->v[i].z);
			if (matched != 3)
			{
				printf("Failed parsing obj file\n");
				exit (1);
			}
		}
		else if(!strcmp(header, "vt"))
		{
			i = result->n_vt;
			result->n_vt++;
			new_prt = realloc (result->vt, sizeof(vertex) * result->n_vt);
			if (new_prt != NULL)
				result->vt = (vertex2 *)new_prt;
			else
			{
				perror("realloc");
				exit(1);
			}
			matched = fscanf(objFile, "%f %f\n", &result->vt[i].x, &result->vt[i].y);
			if (matched != 2)
			{
				printf("Failed parsing obj file\n");
				exit (1);
			}
		}
		else if(!strcmp(header, "vn"))
		{
			i = result->n_vn;
			result->n_vn++;
			new_prt = realloc (result->vn, sizeof(vertex) * result->n_vn);
			if (new_prt != NULL)
				result->vn = (vertex *)new_prt;
			else
			{
				perror("realloc");
				exit(1);
			}
			matched = fscanf(objFile, "%f %f %f\n", &result->vn[i].x, &result->vn[i].y, &result->vn[i].z);
			if (matched != 3)
			{
				printf("Failed parsing obj file\n");
				exit (1);
			}
		}
		else if(!strcmp(header, "f"))
		{
			i = result->n_f;
			result->n_f++;
			new_prt = realloc (result->f, sizeof(vertex) * result->n_f * 3);
			if (new_prt != NULL)
				result->f = (index3 *)new_prt;
			else
			{
				perror("realloc");
				exit(1);
			}
			scanFaceLine(objFile, &result->f[i*3]);
		}
		else /* Consume line */
		{
			int n;
			while ((n = fgetc(objFile)) != '\n' )
				if (n == EOF)
					return 1;
		}
	}

	return 0;
}

int vertexEqual (vertex a, vertex b)
{
	if (a.x != b.x)
		return 0;

	if (a.y != b.y)
		return 0;

	if (a.z != b.z)
		return 0;

	return 1;
}

int vertex2Equal (vertex2 a, vertex2 b)
{
	if (a.x != b.x)
		return 0;

	if (a.y != b.y)
		return 0;

	return 1;
}

int vertexInfoEqual(vertexInfo a, vertexInfo b)
{
	if (!vertexEqual(a.p, b.p))
		return 0;

	if (!vertexEqual(a.n, b.n))
		return 0;

	if (!vertex2Equal(a.t, b.t))
		return 0;

	return 1;
}

int searchVertexInfo(vertexInfo *haystack, int size, vertexInfo needle)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (vertexInfoEqual(haystack[i], needle))
			return i;
	}

	return -1;
}

int load_obj(const char *filename, newMesh *m)
{
	FILE *objFile;
	parseResult parsed;
	int ret, i, filled = 0;
	vertex vNULL = {0,0,0};
	vertex2 v2NULL = {0,0};
	void *new_prt;

	objFile = fopen(filename, "r");
	if (objFile == NULL)
	{
		printf ("Failed to open: %s\n", filename);
		exit(1);
	}
	parseObjFile(objFile, &parsed);

	m->nVerts = m->nIndices = parsed.n_f * 3;
	m->indices = (int *) malloc(m->nIndices * sizeof(int));
	m->vertices = (vertexInfo *) malloc (m->nVerts * sizeof(vertexInfo));

	for (i = 0; i < parsed.n_f * 3; i++)
	{
		vertexInfo vertex_buf;

		vertex_buf.p = parsed.v[parsed.f[i].v - 1];
		vertex_buf.t = (parsed.f[i].vt != 0) ? parsed.vt[parsed.f[i].vt - 1] : v2NULL;
		vertex_buf.n = (parsed.f[i].vn != 0) ? parsed.vn[parsed.f[i].vn - 1] : vNULL;

		ret = searchVertexInfo(m->vertices, filled, vertex_buf);
		if(ret == -1)
		{
			m->vertices[filled] = vertex_buf;
			m->indices[i] = filled;
			filled++;
		}
		else
			m->indices[i] = ret;
	}

	new_prt = realloc(m->vertices, filled * sizeof(vertexInfo));
	if(new_prt != NULL)
		m->vertices = (vertexInfo *) new_prt;

	free(parsed.v);
	free(parsed.vt);
	free(parsed.f);
	free(parsed.vn);

	return 0;
}

