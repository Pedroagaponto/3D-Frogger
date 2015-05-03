#ifndef CORE_H
#define CORE_H

#include <math.h>
#include <stdbool.h>

#define REDUCTION 25
#ifndef M_PI
#define M_PI 3.14159265359
#endif
#define UNUSED_VAR (void)

void drawAxes(void);
void initGrid(void);
void drawGrid(void);
void drawGridNormals(void);
void setProjectionMatrix(void);
void setSegments(int newSegments);
void setWidth(float newWidth);
void setHeight(float newHeight);
void setRotateCamX(float newRotateCamX);
void setRotateCamY(float newRotateCamY);
void setCamZoom(float newCamZoom);
void switchDebug(void);
void switchCartesianFlag(void);
void switchTangentFlag(void);
void switchNormalFlag(void);
int getSegments(void);
float getWidth(void);
float getHeight(void);
float getRotateCamX(void);
float getRotateCamY(void);
float getCamZoom(void);
bool getDebug(void);
bool getCartesianFlag(void);
bool getTangentFlag(void);
bool getNormalFlag(void);

typedef struct{
	float x, y, z;
	float speed, angle;
} projection;

typedef struct {
	projection r0, r;
} frogState;

typedef struct {
	float x, y, z;
} vertex;

extern frogState frog;

#endif

