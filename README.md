# HealthWatch - IoT

Este repositorio contiene el código fuente y la lógica de control del dispositivo wearable (ESP32) para el sistema HealthWatch. Este firmware es responsable de la adquisición de datos biométricos y de movimiento, el procesamiento local en tiempo real y la transmisión eficiente de telemetría mediante el protocolo MQTT.

## Comuniación MQTT

El dispositivo publica los datos bajo el tópico: `healthwatch/{id_patient}/{id_device}/biometrics`, el payload enviado cumple con la siguiente estructura en formato JSON:

```json
{
  "id_patient": "adulto_mayor_42",
  "id_device": "esp32_01",
  "ax": 0.05,
  "ay": -0.12,
  "az": 9.81,
  "gx": 0.0,
  "gy": 0.02,
  "gz": -0.01,
  "temp": 36.6,
  "heart_rate": 72.5,
  "spo2": 98,
  "rssi": -55,
  "battery": 87
}
```

## Comandos básicos

El proyecto usa **PlatformIO CLI**.

- **Build Project:**
  ```bash
  pio run
  ```
- **Upload to ESP32:**
  ```bash
  pio run -t upload
  ```
- **Serial Monitor:**
  ```bash
  pio run -t monitor
  ```
- **Clean Build Files:**
  ```bash
  pio run -t clean
  ```

## Autores

- Chavez Ccahuana Alvaro Andres
- Obando Salinas Enmanuel Jose
- Torres Rivera Richard Maycol
- Vera Alva Miguel Angel
