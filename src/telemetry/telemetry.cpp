#include "telemetry.h"
#include "config.h"
#include "constants.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// Tareas del nucleo 0
static TaskHandle_t sensorTaskHandle;
static void loop0(void *parameter);

// static WiFiClient espClient;
// static PubSubClient mqttClient(espClient);

// setup MQTT
static void reconnectMQTT()
{
    // while (!mqttClient.connected())
    // {
    //     Serial.print("Intentando conexión MQTT...");
    //     // Intentar conectar usando el ID del ESP32 como identificador de cliente MQTT
    //     if (mqttClient.connect(ID_DEVICE))
    //     {
    //         Serial.println("¡Conectado al Broker Mosquitto!");
    //     }
    //     else
    //     {
    //         Serial.print("Falló con estado: ");
    //         Serial.print(mqttClient.state());
    //         Serial.println(". Reintentando en 5 segundos...");
    //         vTaskDelay(pdMS_TO_TICKS(5000));
    //     }
    // }
}

// =========================
// SIMULACIONES
// =========================

static float simularHeartRate()
{
    return random(70, 90);
}

static int simularSpO2()
{
    return random(97, 100);
}

static int simularBateria()
{
    int bat = 100 - (millis() / 60000);
    return (bat < 0) ? 0 : bat;
}

void initTelemetry()
{
    // mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    xTaskCreatePinnedToCore(loop0, "sensorTaskHandle", 5000, NULL, 1, &sensorTaskHandle, 0);
}

static void loop0(void *parameter)
{
    while (true)
    {
        // if (!mqttClient.connected())
        // {
        //     reconnectMQTT();
        // }

        // mqttClient.loop();

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

        // MAX30100
        doc["heart_rate"] = pox.getHeartRate();
        doc["spo2"] = pox.getSpO2();

        // Telemetría
        doc["rssi"] = WiFi.RSSI();
        doc["battery"] = simularBateria();

        // Serializar JSON a un string buffer
        // char jsonBuffer[512];
        // serializeJson(doc, jsonBuffer);

        // Publicar carga útil en el broker MQTT
        // if (mqttClient.publish(MQTT_TOPIC, jsonBuffer))
        // {
        //     Serial.print("Datos transmitidos exitosamente a tópico: ");
        //     Serial.println(MQTT_TOPIC);
        // }
        // else
        // {
        //     Serial.println("Error crítico: No se pudo despachar el mensaje por MQTT.");
        // }

        serializeJsonPretty(doc, Serial);
        Serial.println();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
