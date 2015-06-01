#ifndef CORE_H
#define CORE_H

#include <math.h>
#include <stdbool.h>

#define REDUCTION 25
#ifndef M_PI
#define M_PI 3.14159265359
#endif
#define UNUSED_VAR (void)

typedef struct{
	float x, y, z;
	float r, theta, phi;
	float dx, dy, dz;
} projection;

typedef struct {
	projection r0, r;
	bool onLog;
} frogState;

typedef struct {
	float x, y, z;
} vertex;

void resetGame(void);
void resetPerformance(void);
void setProjectionMatrix(void);
void setupCamera(void);
void drawText(char *text, float x, float y, float z);
void calculatePerformance(void);
void incFrameCount(void);
void setSegments(int newSegments);
void setScore(int newScore);
void setLifes(int newLifes);
void setWidth(int newWidth);
void setHeight(int newHeight);
void setRotateCamTheta(float newRotateCamTheta);
void setRotateCamPhi(float newRotateCamPhi);
void setCamZoom(float newCamZoom);
void updateCartesian(projection *p);
void updateSpherical(projection *p);
void switchDebug(void);
void switchPause(void);
void switchCartesianFlag(void);
void switchNormalFlag(void);
void switchWireFlag(void);
void switchAxesFlag(void);
void switchLightFlag(void);
void switchTextureFlag(void);
void switchVerletFlag(void);
int getSegments(void);
int getScore(void);
int getLifes(void);
int getWidth(void);
int getHeight(void);
int getFps(void);
int getFrameDuration(void);
float getRotateCamTheta(void);
float getRotateCamPhi(void);
float getCamZoom(void);
bool getDebug(void);
bool getPause(void);
bool getCartesianFlag(void);
bool getNormalFlag(void);
bool getWireFlag(void);
bool getAxesFlag(void);
bool getLightFlag(void);
bool getTextureFlag(void);
bool getVerletFlag(void);

extern frogState frog;
extern vertex logs[20];

#endif

