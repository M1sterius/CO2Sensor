#include <Arduino.h>

#include "Utilities/Debug.hpp"
#include "../../Protocol/Protocol.hpp"
#include "Network/Connection.hpp"
#include "Network/NetworkTask.hpp"
#include "Sensor/Sensor.hpp"
#include "DataSaver.hpp"
#include "Display.hpp"
#include "Config.hpp"

using namespace CO2;
using namespace CO2::Firmware;

constexpr uint8_t CONFIGURATION_BUTTON_PIN = 18;

/* -------------- Global instances --------------*/
Config g_Config;
Sensor g_Sensor;
Connection g_Connection;
NetworkTask g_NetworkTask;
DataSaver g_DataSaver;
Display g_Display;
/* -------------- Global instances --------------*/

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
    pinMode(CONFIGURATION_BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(SERIAL_BAUD);
    g_Display.Begin();

    if (!g_Config.Begin())
        FatalError("Failed to initialize config!", 10);

    if (!digitalRead(CONFIGURATION_BUTTON_PIN) || g_Config.IsConfigurationRequired())
    {
        g_Display.ClearPrintf("Configuration!");
        g_Config.Configure();
    }

    g_Display.ClearPrintf("Starting up...");
    vTaskDelay(pdMS_TO_TICKS(3000)); // TODO: Remove. Only for debug
    
    if (g_Sensor.Begin())
        DEBUG_LOG("Sensor successfully initialized!");
    else
        FatalError("Sensor failed to initialize.", 11);

    if (g_DataSaver.Begin())
        DEBUG_LOG("Data saver successfully initialized!");
    else
        FatalError("Data saver failed to initialize.", 12);

    g_Display.ClearPrintf("Connecting...");
    g_Connection.SetConfigInstance(&g_Config);
    g_Connection.Begin();

    if (!g_Connection.IsTimeConfigured())
        DEBUG_LOG("Readings won't be saved to flash because time could not be configured via NTP!");

    g_Sensor.BeginTask();
    g_NetworkTask.Begin(g_Sensor.GetQueue(), &g_Connection, &g_DataSaver);
}

void loop()
{
    static float temp, hum;
    static uint32_t co2;
    g_Sensor.GetDisplayStats(temp, hum, co2);
    g_Display.DrawDashboard(temp, hum, co2);

    vTaskDelay(pdMS_TO_TICKS(5000));
}
