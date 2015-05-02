#include <stdio.h>

#include <GL/glut.h>

#include "sphere.h"
#include "core.h"

void drawSphere(void)
{
	glColor3f (1, 1, 1);
	if (getDebug())
		printf(">>>>SPHERE<<<<<\n");
	glTranslatef(frog.r.x, frog.r.y, 0);
	glScalef(0.1f, 0.1f, 0.1f);
	glutWireSphere(1, 10, 10);
}

