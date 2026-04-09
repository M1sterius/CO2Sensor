#include <Arduino.h>
#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <ESPmDNS.h>

#include "Utilities/Debug.hpp"
#include "../../Protocol/Protocol.hpp"
#include "Network/Connection.hpp"
#include "Network/NetworkTask.hpp"
#include "Sensor/Sensor.hpp"
#include "DataSaver.hpp"
#include "Display.hpp"

using namespace CO2;
using namespace CO2::Firmware;

constexpr uint32_t BUTTON_DEBOUNCE_DELAY = 50;
constexpr uint8_t BUTTON_PIN = 18;

/* -------------- Global instances --------------*/
Sensor g_Sensor;
Connection g_Connection;
NetworkTask g_NetworkTask;
DataSaver g_DataSaver;
Display g_Display;
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

void FatalError(const char* text, const int code)
{
    auto& disp = g_Display.Get();

    while (true)
    {
        Serial.printf("Fatal error! Code: %i. %s\n", code, text);

        disp.clear();

        disp.setCursor(0, 0);
        disp.print("Fatal error!");

        disp.setCursor(0, 1);
        disp.printf("Code: %i", code);

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void setup()
{
    Serial.begin(SERIAL_BAUD);
    g_Display.Begin();

    g_Display.ClearPrintf("Starting up...");
    vTaskDelay(pdMS_TO_TICKS(3000)); // TODO: Remove. Only for debug
    
    if (g_Sensor.Begin())
        DEBUG_LOG("Sensor successfully initialized!");
    else
        FatalError("Sensor failed to initialize.", 10);

    if (g_DataSaver.Begin())
        DEBUG_LOG("Data saver successfully initialized!");
    else
        FatalError("Data saver failed to initialize.", 11);

    g_Display.ClearPrintf("Connecting...");
    g_Connection.Begin();

    if (!g_Connection.IsTimeConfigured())
        DEBUG_LOG("Readings won't be saved to flash because time could not be configured via NTP!");

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, ButtonCallback, HIGH);

    g_Sensor.BeginTask();
    g_NetworkTask.Begin(g_Sensor.GetQueue(), &g_Connection, &g_DataSaver);
}

void loop()
{
    if (g_ButtonPressed)
    {
        g_Connection.Terminate();
        g_ButtonPressed = false;
    }

    static float temp, hum;
    static uint32_t co2;
    g_Sensor.GetDisplayStats(temp, hum, co2);
    g_Display.DrawDashboard(temp, hum, co2);

    vTaskDelay(pdMS_TO_TICKS(2000));
}
