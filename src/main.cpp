#include "config.h"
#include "constants.h"
#include "states.h"
#include "init/init.h"
#include "draw/draw.h"
#include "items/items.h"

// Tareas del nucleo 0
TaskHandle_t sensorTaskHandle;
void loop0(void *parameter);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// setup MQTT
void reconnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Intentando conexión MQTT...");
        // Intentar conectar usando el ID del ESP32 como identificador de cliente MQTT
        if (mqttClient.connect(ID_DEVICE))
        {
            Serial.println("¡Conectado al Broker Mosquitto!");
        }
        else
        {
            Serial.print("Falló con estado: ");
            Serial.print(mqttClient.state());
            Serial.println(". Reintentando en 5 segundos...");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}

// =========================
// SIMULACIONES
// =========================

float simularHeartRate()
{
    return random(70, 90);
}

int simularSpO2()
{
    return random(97, 100);
}

int simularBateria()
{
    int bat = 100 - (millis() / 60000);
    return (bat < 0) ? 0 : bat;
}

// Manejo de pantallas
byte currentScreen = SCREEN_CLOCK;
byte previousScreen = -1;

// Items
int items[] = {ITEM_CHRONOMETER, ITEM_FLASHLIGHT, ITEM_EXIT};

// Botones
bool pressSelect = false;
bool pressUp = false;
bool pressDown = false;

// Tiempo cronometro
bool runningChronometer = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

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
    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    xTaskCreatePinnedToCore(loop0, "sensorTaskHandle", 5000, NULL, 1, &sensorTaskHandle, 0);
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

void downAction()
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

void updateScreen()
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

void loop0(void *parameter)
{
    while (true)
    {
        if (!mqttClient.connected())
        {
            reconnectMQTT();
        }

        mqttClient.loop();

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        JsonDocument doc;

        // TAGS
        doc["id_patient"] = ID_PATIENT;
        doc["id_device"] = ID_DEVICE;

        // MPU6050
        doc["ax"] = a.acceleration.x;
        doc["ay"] = a.acceleration.y;
        doc["az"] = a.acceleration.z;

        doc["gx"] = g.gyro.x;
        doc["gy"] = g.gyro.y;
        doc["gz"] = g.gyro.z;

        doc["temp"] = temp.temperature;

        /// Biométricos simulados
        doc["heart_rate"] = simularHeartRate();
        doc["spo2"] = simularSpO2();

        // Telemetría
        doc["rssi"] = WiFi.RSSI();
        doc["battery"] = simularBateria();

        // Serializar JSON a un string buffer
        char jsonBuffer[512];
        serializeJson(doc, jsonBuffer);

        // Publicar carga útil en el broker MQTT
        if (mqttClient.publish(MQTT_TOPIC, jsonBuffer))
        {
            Serial.print("Datos transmitidos exitosamente a tópico: ");
            Serial.println(MQTT_TOPIC);
        }
        else
        {
            Serial.println("Error crítico: No se pudo despachar el mensaje por MQTT.");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}