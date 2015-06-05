#ifndef JUMPING_LOGIC_H
#define JUMPING_LOGIC_H

#include <stdbool.h>

#define GRAVITY 20

void switchAnalyticFlag(void);
bool gameCheck(void);
void jumpingSettings(void);
int jumpingIdle(void);
float calcReach(void);
float calcTimeOfFlight(void);
bool parabolaInsideWindow(void);

#endif

