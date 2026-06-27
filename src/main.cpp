#include "config.h"
#include "init/init.h"
#include "telemetry/telemetry.h"
#include "input/input.h"
#include "state/state_manager.h"
#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
    initWifi();
    Serial.println("-----------------------------");
    initNtpClient();
    Serial.println("-----------------------------");
    initButtons();
    Serial.println("-----------------------------");
    initScreen();
    Serial.println("-----------------------------");
    initMPU();
    Serial.println("-----------------------------");

    initTelemetry();
    initState();
}

void loop()
{
    updateInputs(onSelectPressed, onUpPressed, onDownPressed);
    updateState();
}