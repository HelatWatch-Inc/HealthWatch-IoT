#ifndef ITEMS_H
#define ITEMS_H

#include <Arduino.h>

// Cronometro
void startPauseChronometer(bool &run, unsigned long &start, unsigned long &elapsed);
void exitChronometer(bool &run, unsigned long &start, unsigned long &elapsed);
void resetChronometer(bool &run, unsigned long &start, unsigned long &elapsed);

#endif