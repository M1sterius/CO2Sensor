#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <Adafruit_BMP085.h>

#include "Debug.hpp"
#include "../../Protocol/Protocol.hpp"
#include "Network/Connection.hpp"
#include "Network/NetworkTask.hpp"
#include "Sensor/Sensor.hpp"

using namespace CO2;
using namespace CO2::Firmware;

constexpr uint32_t BUTTON_DEBOUNCE_DELAY = 50;
constexpr uint8_t BUTTON_PIN = 18;

/* -------------- Global instances --------------*/
Sensor g_Sensor;
Connection g_Connection;
NetworkTask g_NetworkTask;
/* -------------- Global instances --------------*/

volatile bool g_ButtonPressed = false;
volatile uint32_t g_ButtonPressedTime = 0;

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

    if (g_Connection.Begin())
        DEBUG_LOG("Server connection established!");
    else
        DEBUG_LOG("Failed to establish server connection!");

    g_NetworkTask.Begin(g_Sensor.GetQueue(), &g_Connection);
}

void loop()
{
    if (g_ButtonPressed)
    {
        g_Connection.Terminate();
        g_ButtonPressed = false;
    }
}
