#include "telemetry.h"
#include "config.h"
#include "constants.h"
#include "secrets.h"

// Variables globales para IDs dinamicos
char id_device[40] = DEFAULT_ID_DEVICE;
char id_patient[40] = DEFAULT_ID_PATIENT;
char mqtt_topic[100] = "healthwatch/" DEFAULT_ID_PATIENT "/" DEFAULT_ID_DEVICE "/biometrics";

// Tareas del nucleo 0
static TaskHandle_t sensorTaskHandle;
static void loop0(void *parameter);

static WiFiClientSecure espClient;
static PubSubClient mqttClient(espClient);

// setup MQTT
static void reconnectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.print("Intentando conexión MQTT...");
        if (mqttClient.connect(id_device, MQTT_USER, MQTT_PASS))
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

static void oximeterTask(void *parameter)
{
    while (true)
    {
        if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE)
        {
            pox.update();
            xSemaphoreGive(i2cMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(5)); // Actualizar cada 5 ms
    }
}

void initTelemetry()
{
    mqttClient.setBufferSize(MAX_MQTT_PACKET_SIZE);
    xTaskCreatePinnedToCore(loop0, "sensorTaskHandle", 5000, NULL, 1, &sensorTaskHandle, 0);
    xTaskCreatePinnedToCore(oximeterTask, "oximeterTask", 3000, NULL, 2, NULL, 0);
}

static void loop0(void *parameter)
{
    espClient.setCACert(mqtt_ca_cert);

    mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
    while (true)
    {
        if (!mqttClient.connected())
        {
            reconnectMQTT();
        }

        mqttClient.loop();

        sensors_event_t a, g, temp;
        if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE)
        {
            mpu.getEvent(&a, &g, &temp);
            xSemaphoreGive(i2cMutex);
        }

        JsonDocument doc;

        // TAGS
        doc["id_patient"] = id_patient;
        doc["id_device"] = id_device;

        // MPU6050
        doc["ax"] = a.acceleration.x;
        doc["ay"] = a.acceleration.y;
        doc["az"] = a.acceleration.z;

        doc["gx"] = g.gyro.x;
        doc["gy"] = g.gyro.y;
        doc["gz"] = g.gyro.z;

        doc["temp"] = temp.temperature;

        // MAX30100
        float hr = 0.0;
        float sp = 0.0;
        if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE)
        {
            hr = pox.getHeartRate();
            sp = pox.getSpO2();
            xSemaphoreGive(i2cMutex);
        }
        doc["heart_rate"] = hr;
        doc["spo2"] = sp;

        // Telemetría
        doc["rssi"] = WiFi.RSSI();
        doc["battery"] = simularBateria();

        // Serializar JSON a un string buffer
        char jsonBuffer[MAX_MQTT_PACKET_SIZE];
        serializeJson(doc, jsonBuffer);

        // Publicar carga útil en el broker MQTT
        if (mqttClient.publish(mqtt_topic, jsonBuffer))
        {
            Serial.print("Datos transmitidos exitosamente a tópico: ");
            Serial.println(mqtt_topic);
        }
        else
        {
            Serial.println("Error crítico: No se pudo despachar el mensaje por MQTT.");
            Serial.print("Estado del cliente MQTT: ");
            Serial.println(mqttClient.state());
            Serial.print("Tamanio del JSON generado:");
            Serial.println(strlen(jsonBuffer));
        }

        serializeJsonPretty(doc, Serial);
        Serial.println();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
