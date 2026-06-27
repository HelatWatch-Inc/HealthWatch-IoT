#include "state_manager.h"
#include "config.h"
#include "constants.h"
#include "states.h"
#include "draw/draw.h"
#include "items/items.h"
#include <Arduino.h>

// Manejo de pantallas
static byte currentScreen = SCREEN_CLOCK;
static byte previousScreen = -1;

// Items
static int items[] = {ITEM_CHRONOMETER, ITEM_FLASHLIGHT, ITEM_EXIT};

// Tiempo cronometro
static bool runningChronometer = false;
static unsigned long startTime = 0;
static unsigned long elapsedTime = 0;

static void updateScreen()
{
    if (currentScreen != previousScreen)
    {
        switch (currentScreen)
        {
        case SCREEN_CLOCK:
            drawClock(true);
            break;
        case SCREEN_MENU:
            if (previousScreen != SCREEN_ITEM)
            {
                // Reinicio de visualizion de items
                items[ITEM_PREVIOUS] = ITEM_CHRONOMETER;
                items[ITEM_SELECTED] = ITEM_FLASHLIGHT;
                items[ITEM_NEXT] = ITEM_EXIT;
            }
            drawMenu(true, items);
            break;
        case SCREEN_ITEM:
            switch (items[ITEM_SELECTED])
            {
            case ITEM_CHRONOMETER:
                drawChronometer(true, elapsedTime);
                break;
            case ITEM_FLASHLIGHT:
                drawFlashlight(true);
                break;
            default:
                break;
            }
            break;
        }
        previousScreen = currentScreen;
    }
    else
    {
        switch (currentScreen)
        {
        case SCREEN_CLOCK:
            drawClock(false);
            break;
        case SCREEN_MENU:
            items[ITEM_PREVIOUS] = items[ITEM_SELECTED] - 1;
            if (items[ITEM_PREVIOUS] < 0)
            {
                items[ITEM_PREVIOUS] = NUM_MENU_ITEMS - 1;
            }
            items[ITEM_NEXT] = items[ITEM_SELECTED] + 1;
            if (items[ITEM_NEXT] >= NUM_MENU_ITEMS)
            {
                items[ITEM_NEXT] = 0;
            }
            drawMenu(false, items);
            break;
        case SCREEN_ITEM:
            switch (items[ITEM_SELECTED])
            {
            case ITEM_CHRONOMETER:
                drawChronometer(false, elapsedTime);
                break;
            case ITEM_FLASHLIGHT:
                drawFlashlight(false);
                break;
            default:
                break;
            }
            break;
        }
    }
}

void initState()
{
    currentScreen = SCREEN_CLOCK;
    previousScreen = -1;
    runningChronometer = false;
    startTime = 0;
    elapsedTime = 0;
}

void updateState()
{
    if (runningChronometer)
    {
        elapsedTime = millis() - startTime;
    }
    updateScreen();
}

void onSelectPressed()
{
    Serial.println("Boton Select apretado");
    if (currentScreen == SCREEN_CLOCK)
    {
        currentScreen = SCREEN_MENU;
    }
    else if (currentScreen == SCREEN_MENU)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_EXIT:
            currentScreen = SCREEN_CLOCK;
            break;
        default:
            currentScreen = SCREEN_ITEM;
            break;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_CHRONOMETER:
            // exit
            exitChronometer(runningChronometer, startTime, elapsedTime);
            currentScreen = SCREEN_MENU;
            break;
        default:
            currentScreen = SCREEN_MENU;
            break;
        }
    }
}

void onUpPressed()
{
    Serial.println("Boton Up apretado");
    if (currentScreen == SCREEN_MENU)
    {
        items[ITEM_SELECTED]--;
        if (items[ITEM_SELECTED] < 0)
        {
            items[ITEM_SELECTED] = NUM_MENU_ITEMS - 1;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_CHRONOMETER:
            startPauseChronometer(runningChronometer, startTime, elapsedTime);
            break;
        case ITEM_FLASHLIGHT:
            currentScreen = SCREEN_MENU;
            break;
        default:
            break;
        }
    }
}

void onDownPressed()
{
    Serial.println("Boton Down apretado");
    if (currentScreen == SCREEN_MENU)
    {
        items[ITEM_SELECTED]++;
        if (items[ITEM_SELECTED] >= NUM_MENU_ITEMS)
        {
            items[ITEM_SELECTED] = 0;
        }
    }
    else if (currentScreen == SCREEN_ITEM)
    {
        switch (items[ITEM_SELECTED])
        {
        case ITEM_CHRONOMETER:
            // reset
            resetChronometer(runningChronometer, startTime, elapsedTime);
            break;
        case ITEM_FLASHLIGHT:
            currentScreen = SCREEN_MENU;
            break;
        default:
            break;
        }
    }
}
