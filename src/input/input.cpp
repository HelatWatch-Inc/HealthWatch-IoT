#include "input.h"
#include "config.h"
#include <Arduino.h>

static bool pressSelect = false;
static bool pressUp = false;
static bool pressDown = false;

static void handleButtonPress(int button, bool &pressFlag, void (*action)())
{
    if (digitalRead(button) == LOW)
    {
        pressFlag = true;
    }
    if (digitalRead(button) == HIGH && pressFlag)
    {
        pressFlag = false;
        action();
    }
}

void updateInputs(void (*selectCallback)(), void (*upCallback)(), void (*downCallback)())
{
    handleButtonPress(BUTTON_SELECT, pressSelect, selectCallback);
    handleButtonPress(BUTTON_UP, pressUp, upCallback);
    handleButtonPress(BUTTON_DOWN, pressDown, downCallback);
}
