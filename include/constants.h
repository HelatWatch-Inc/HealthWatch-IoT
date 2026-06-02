#ifndef CONSTANTS_H
#define CONSTANTS_H

#pragma once

// Wifi
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASS ""

// MQTT
#define MQTT_BROKER "192.168.18.16" // ip de la máquina
#define MQTT_PORT 1883

// Identificadores
#define ID_DEVICE "esp32_01"
#define ID_PATIENT "patient_01"

#define MQTT_TOPIC "healthwatch/" ID_PATIENT "/" ID_DEVICE "/biometrics"

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