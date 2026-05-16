#ifndef DRAW_H
#define DRAW_H

#include "config.h"
#include "constants.h"
#include "states.h"

// Pantallas
void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
// Items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);
void drawFlashlight(bool fullRedraw);

#endif
