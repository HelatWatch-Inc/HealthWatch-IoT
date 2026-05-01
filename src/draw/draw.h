#ifndef DRAW_H
#define DRAW_H

// Pantallas
void drawClock(bool fullRedraw);
void drawMenu(bool fullRedraw, int items[]);
// Items
void drawChronometer(bool fullRedraw, unsigned long elapsedTime);

#endif
