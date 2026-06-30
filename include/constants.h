#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma once

// Configuración de Menú
const byte NUM_MENU_ITEMS = 3;
const byte MAX_MENU_ITEM_LENGTH = 20;

// Calendario
extern const char *daysOfTheWeek[7];
extern const char *monthsOfTheYear[12];

// Items
extern char menuItems[NUM_MENU_ITEMS][MAX_MENU_ITEM_LENGTH];
extern const unsigned char *icons_bitmaps[NUM_MENU_ITEMS];

#endif