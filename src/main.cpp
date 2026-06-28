#include "config.h"
#include "init/init.h"
#include "telemetry/telemetry.h"
#include "input/input.h"
#include "state/state_manager.h"
#include <Arduino.h>

SemaphoreHandle_t i2cMutex = NULL;

void setup()
{
    Serial.begin(9600);
    
    // Inicializar el mutex I2C antes de inicializar cualquier periferico I2C
    i2cMutex = xSemaphoreCreateMutex();
    
    initWifi();
    Serial.println("-----------------------------");
    initNtpClient();
    Serial.println("-----------------------------");
    initButtons();
    Serial.println("-----------------------------");

    Wire.begin(I2C_SDA, I2C_SCL);
    initMPU();
    Serial.println("-----------------------------");
    initScreen();
    Serial.println("-----------------------------");
    initPulseOximeter();
    Serial.println("-----------------------------");

    initTelemetry();
    initState();
}

void loop()
{
    updateInputs(onSelectPressed, onUpPressed, onDownPressed);
    updateState();
}