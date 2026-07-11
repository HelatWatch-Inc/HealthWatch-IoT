#ifndef CONFIG_H
#define CONFIG_H

/****************************************************************
                       LIBRERIAS NECESARIAS
****************************************************************/
// Comunicacions 12C
#include <Wire.h>
#include <freertos/semphr.h>

// Manejo de graficos
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Conexion a Wifi
#include <WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>

//  Cliente NTP
#include <NTPClient.h>

// Sensores
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "MAX30100_PulseOximeter.h"

// MQTT
#define MAX_MQTT_PACKET_SIZE 512
#include <PubSubClient.h>

// JSON
#include <ArduinoJson.h>

/****************************************************************
                       PINES DE HARDWARE
****************************************************************/
// Pantalla OLED
#define OLED_RESET -1

// BOTONES
#define BUTTON_SELECT 12
#define BUTTON_UP 13
#define BUTTON_DOWN 14

// BUZZER
#define BUZZER 4

// I2C
#define I2C_SDA 21
#define I2C_SCL 22

/****************************************************************
                        CONFIGURACION DE PANTALLA
****************************************************************/
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

/****************************************************************
                        OBJETOS GLOBALES
****************************************************************/
extern Adafruit_SSD1306 display;
extern WiFiUDP ntpUDP;
extern NTPClient ntpClient;
extern Adafruit_MPU6050 mpu;
extern PulseOximeter pox;
extern SemaphoreHandle_t i2cMutex;

#endif
