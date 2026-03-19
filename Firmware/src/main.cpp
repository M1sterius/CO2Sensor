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
    while (true)
    {
        Serial.printf("Fatal error! Code: %i. %s\n", code, text);
        g_Display.ClearPrintf("Fatal error!\nCode: %i", code);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup()
{
    Serial.begin(115200);
    g_Display.Begin();

    g_Display.ClearPrintf("Starting up...");
    
    if (g_Sensor.Begin()) {
        DEBUG_LOG("Sensor successfully initialized!");
    } else {
        CATASTROPHIC_ERROR("Catastrophic error! Sensor failed to initialize."); 
    }

    if (g_DataSaver.Begin()) { 
        DEBUG_LOG("Data saver successfully initialized!");
    } else {
        CATASTROPHIC_ERROR("Catastrophic error! Data saver failed to initialize.");
    }

    g_Display.ClearPrintf("Connecting...");
    if (g_Connection.Begin()) { 
        DEBUG_LOG("Network connection established!");
        g_Display.ClearPrintf("Connected!");
    } else {
        g_Display.ClearPrintf("No connection!");
        DEBUG_LOG("Failed to establish network connection!");
    }

    vTaskDelay(pdMS_TO_TICKS(200));

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

    static float temp, hum, co2;
    g_Sensor.GetDisplayStats(temp, hum, co2);

    auto& disp = g_Display.Get();
    disp.clear();
    
    disp.setCursor(0, 0);
    disp.printf("CO2: %.2f PPM", co2);

    disp.setCursor(0, 1);
    disp.printf("T: %.1f C, H: %.1f", temp, hum);

    vTaskDelay(pdMS_TO_TICKS(2000));
}
