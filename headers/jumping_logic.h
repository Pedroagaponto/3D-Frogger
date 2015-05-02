#ifndef JUMPING_LOGIC_H
#define JUMPING_LOGIC_H

#include <stdbool.h>

#define GRAVITY 9.8

bool getJumpingFlag(void);
void switchAnalyticFlag(void);
void jumpingSettings(void);
int jumpingIdle(void);
float calcReach(void);
void calcPosition(float t, float dt);
bool parabolaInsideWindow(void);

#endif

