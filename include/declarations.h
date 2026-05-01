#ifndef DECLARATIONS_H
#define DECLARATIONS_H

/****************************************************************
                       LIBRERIAS NECESARIAS
****************************************************************/
// Comunicacions 12C
#include <Wire.h>
// Manejo de graficos
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Conexion a Wifi
#include <WiFi.h>
#include <WiFiUdp.h>
//  Cliente NTP
#include <NTPClient.h>
// Vectores
#include <vector>

/****************************************************************
                       PINES DE HARDWARE
****************************************************************/
// Pantalla OLED
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RESET -1
// BOTONES
#define BUTTON_SELECT 12
#define BUTTON_UP 13
#define BUTTON_DOWN 14

/****************************************************************
                        CONFIGURACION DE PANTALLA
****************************************************************/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
extern Adafruit_SSD1306 display;

/****************************************************************
                       CONFIGURACION DE WIFI
****************************************************************/
extern WiFiUDP ntpUDP;
extern NTPClient ntpClient;

/****************************************************************
                       CONFIGURACION DE FECHAS
****************************************************************/
extern const char *daysOfTheWeek[7];
// Array de meses en español
extern const char *monthsOfTheYear[12];

/****************************************************************
                       PANTALLAS
****************************************************************/
#define SCREEN_CLOCK 0
#define SCREEN_MENU 1
#define SCREEN_ITEM 2

/****************************************************************
                       ITEMS
****************************************************************/
// Posicion en pantalla
#define ITEM_PREVIUS 0
#define ITEM_SELECTED 1
#define ITEM_NEXT 2

// Arreglo de items
const int NUM_ITEMS = 3;
const int MAX_ITEM_LENGTH = 20;
extern char menuItems[NUM_ITEMS][MAX_ITEM_LENGTH];
extern const unsigned char *icons_bitmaps[NUM_ITEMS];

/****************************************************************
                       MENU DE ITEMS
****************************************************************/
#define ITEM_LINTERN 0
#define ITEM_CHRONOMETER 1
#define ITEM_EXIT 2

#endif
