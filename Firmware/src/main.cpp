#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <Adafruit_BMP085.h>

#include "Debug.hpp"
#include "../../Protocol/Protocol.hpp"
#include "Network/Connection.hpp"
#include "Sensor/Sensor.hpp"

using namespace CO2;
using namespace CO2::Firmware;

constexpr uint32_t BUTTON_DEBOUNCE_DELAY = 50;
constexpr uint8_t BUTTON_PIN = 18;

QueueHandle_t g_SensorQueue;
Sensor g_Sensor;
Connection g_Connection;

volatile bool g_ButtonPressed = false;
volatile uint32_t g_ButtonPressedTime = 0;

void NetworkTask(void* pvParameters)
{
    const TickType_t period = pdMS_TO_TICKS(1000);
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        if (g_Connection.Connected())
        {
            Serial.println("Doing network tasks!");
        }
        else
        {
            Serial.println("No connection!");
            g_Connection.Begin();
        }

        vTaskDelayUntil(&lastWakeTime, period);
    }
}

void ARDUINO_ISR_ATTR ButtonCallback()
{
    const auto now = millis();

    if (now - g_ButtonPressedTime > BUTTON_DEBOUNCE_DELAY)
        g_ButtonPressed = true;

    g_ButtonPressedTime = now;
}

void setup()
{
    delay(3000); // to make sure I have enough time to open serial monitor
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, ButtonCallback, HIGH);

    if (g_Connection.Begin())
        DEBUG_LOG("Server connection established!");
    else
        DEBUG_LOG("Failed to establish server connection!");
        
    if (g_Sensor.Begin())
        DEBUG_LOG("Sensor successfully initialized!");
    else
    {
        while (true)
        {
            DEBUG_LOG("Sensor failed to initialize. Catastrophic error!");
            delay(1000);
        }
    }

    g_SensorQueue = xQueueCreate(10, sizeof(SensorData));
    while (!g_SensorQueue)
    {
        DEBUG_LOG("Failed to create sensor data queue. Catastrophic error!");
        delay(500);
    }
    
    xTaskCreatePinnedToCore(NetworkTask, "NetworkTask", 4096, NULL, 1, NULL, 1);
}

void loop()
{
    if (g_ButtonPressed)
    {
        g_Connection.Terminate();
        g_ButtonPressed = false;
    }
}
