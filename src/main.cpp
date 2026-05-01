#include "declarations.h"
#include "init/init.h"
#include "draw/draw.h"
#include "items/items.h"

// Manejo de pantallas
byte currentScreen = SCREEN_CLOCK;
byte previusScreen = -1;
// Items
int items[] = {ITEM_LINTERN, ITEM_CHRONOMETER, ITEM_EXIT};

// Botones
bool pressSelect = false;
bool pressUp = false;
bool pressDown = false;

// Tiempo cronometro
bool runningChronometer = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

// Linterna
bool linter_on = false;

// Progreso
unsigned short progress = 0;

void setup()
{
  Serial.begin(9600);
  initWifi();
  Serial.println("-----------------------------");
  initNtpClient();
  Serial.println("-----------------------------");
  initButtons();
  Serial.println("-----------------------------");
  Serial.println("-----------------------------");
  initScreen();
  Serial.println("-----------------------------");
}

void handleButtonPress(int button, bool &pressFlag, void (*action)())
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

void selectAction()
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
    case ITEM_LINTERN:
      linter_on = !linter_on;
      Serial.println("Linterna " + String(linter_on ? "encendida" : "apagada"));
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

void upAction()
{
  Serial.println("Boton Up apretado");
  if (currentScreen == SCREEN_MENU)
  {
    items[ITEM_SELECTED]--;
    if (items[ITEM_SELECTED] < 0)
    {
      items[ITEM_SELECTED] = NUM_ITEMS - 1;
    }
  }
  else if (currentScreen == SCREEN_ITEM)
  {
    switch (items[ITEM_SELECTED])
    {
    case ITEM_CHRONOMETER:
      startPauseChronometer(runningChronometer, startTime, elapsedTime);
      break;
    default:
      break;
    }
  }
}

void downAction()
{
  Serial.println("Boton Down apretado");
  if (currentScreen == SCREEN_MENU)
  {
    items[ITEM_SELECTED]++;
    if (items[ITEM_SELECTED] >= NUM_ITEMS)
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
    default:
      break;
    }
  }
}

void updateScreen()
{
  if (currentScreen != previusScreen)
  {
    switch (currentScreen)
    {
    case SCREEN_CLOCK:
      drawClock(true);
      break;
    case SCREEN_MENU:
      if (previusScreen != SCREEN_ITEM)
      {
        // Reinicio de visualizion de items
        items[ITEM_PREVIUS] = ITEM_LINTERN;
        items[ITEM_SELECTED] = ITEM_CHRONOMETER;
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
      default:
        break;
      }
      break;
    }
    previusScreen = currentScreen;
  }
  else
  {
    switch (currentScreen)
    {
    case SCREEN_CLOCK:
      drawClock(false);
      break;
    case SCREEN_MENU:
      items[ITEM_PREVIUS] = items[ITEM_SELECTED] - 1;
      if (items[ITEM_PREVIUS] < 0)
      {
        items[ITEM_PREVIUS] = NUM_ITEMS - 1;
      }
      items[ITEM_NEXT] = items[ITEM_SELECTED] + 1;
      if (items[ITEM_NEXT] >= NUM_ITEMS)
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
      default:
        break;
      }
      break;
    }
  }
}

void loop()
{
  handleButtonPress(BUTTON_SELECT, pressSelect, selectAction);
  handleButtonPress(BUTTON_UP, pressUp, upAction);
  handleButtonPress(BUTTON_DOWN, pressDown, downAction);

  if (runningChronometer)
  {
    elapsedTime = millis() - startTime;
  }

  updateScreen();
}