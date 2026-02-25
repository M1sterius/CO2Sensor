#include "Connection.hpp"
#include "../Debug.hpp"
#include "../../Protocol/Protocol.hpp"

static void NetworkTask(void* pvParameters)
{
    auto queue = static_cast<QueueHandle_t>(pvParameters);

    const auto period = pdMS_TO_TICKS(1000);
    auto lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        vTaskDelayUntil(&lastWakeTime, period);
    }
}

namespace CO2::Firmware
{
    Connection::Connection() = default;
    Connection::~Connection() = default;

    bool Connection::Begin()
    {
        uint32_t count = 0;

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED && count < MAX_CONNECT_ATTEMPTS)
        {
            DEBUG_LOG("Attempting to connect to WiFi!");
            delay(CONNECT_ATTEMPT_DELAY);
            count++;
        }
        
        if (WiFi.status() != WL_CONNECTED)
        {
            DEBUG_LOG("Failed to connect to WiFi.");
            return false;
        }
        else
            DEBUG_LOG("WiFi connected!");

        count = 0;
        while (!m_WiFiClient.connect(SERVER_IP, SERVER_PORT) && count < MAX_CONNECT_ATTEMPTS)
        {
            DEBUG_LOG("Attempting to connect to the server.");
            delay(CONNECT_ATTEMPT_DELAY);
            count++;
        }
        
        if (!m_WiFiClient.connected())
        {
            DEBUG_LOG("Failed to connect to the server!");
            return false;
        }
        else
            DEBUG_LOG("Server connected!");

        return true;
    }

    void Connection::Terminate()
    {
        m_WiFiClient.stop();
        WiFi.disconnect();

        DEBUG_LOG("Connection terminated!");
    }

    bool Connection::Connected() { return WiFiConnected() && ServerConnected(); }
    bool Connection::ServerConnected() { return m_WiFiClient.connected(); }
    bool Connection::WiFiConnected() { return WiFi.status() == WL_CONNECTED; }
}
