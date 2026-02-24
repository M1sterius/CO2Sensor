#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <Adafruit_BMP085.h>

#include "Debug.hpp"
#include "../../Protocol/Protocol.hpp"
#include "Network/Connection.hpp"

using namespace CO2;
using namespace CO2::Firmware;

constexpr uint32_t BUTTON_DEBOUNCE_DELAY = 50;
constexpr uint8_t BUTTON_PIN = 18;

Adafruit_BMP085 bmp;
Connection connection;

volatile bool g_ButtonPressed = false;
volatile uint32_t g_ButtonPressedTime = 0;

void NetworkTask(void* pvParameters)
{
    const TickType_t period = pdMS_TO_TICKS(1000);
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        if (connection.Connected())
        {
            Serial.println("Doing network tasks!");
        }
        else
        {
            Serial.println("No connection!");
            connection.Begin();
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

    if (!connection.Begin())
        DEBUG_LOG("Failed to establish connection!");
    else
        DEBUG_LOG("Connection established!");
        
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, ButtonCallback, HIGH);

    xTaskCreatePinnedToCore(
        NetworkTask,
        "NetworkTask",
        4096,
        NULL,
        1,
        NULL,
        1
    );
}

void loop()
{
    if (g_ButtonPressed)
    {
        connection.Terminate();
        g_ButtonPressed = false;
    }
}
