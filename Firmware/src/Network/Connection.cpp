#include "Connection.hpp"
#include "Utilities/Debug.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    Connection::Connection() = default;
    Connection::~Connection() = default;

    bool Connection::Begin()
    {
        return ConnectWiFi() && ConnectServer();
    }

    void Connection::Terminate()
    {
        m_WiFiClient.stop();
        WiFi.disconnect();

        DEBUG_LOG("Connection terminated!");
    }

    void Connection::Reconnect()
    {
        if (!WiFi.isConnected())
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        
        m_WiFiClient.connect(SERVER_IP, SERVER_PORT);
    }

    bool Connection::Connected()
    {
        return WiFi.isConnected() && m_WiFiClient.connected();
    }

    void Connection::Print(const char* str)
    {
        if (m_WiFiClient.connected())
            m_WiFiClient.print(str);
    }

    void Connection::Println(const char* str)
    {
        if (m_WiFiClient.connected())
            m_WiFiClient.println(str);
    }

    bool Connection::ConnectWiFi()
    {
        uint32_t count = 0;

        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (!WiFi.isConnected() && count < MAX_CONNECT_ATTEMPTS)
        {
            DEBUG_LOG("Attempting to connect to WiFi!");
            delay(CONNECT_ATTEMPT_DELAY);
            count++;
        }
        
        if (!WiFi.isConnected())
        {
            DEBUG_LOG("Failed to connect to WiFi.");
            return false;
        }
        else
        {
            DEBUG_LOG("WiFi connected!");
            return true;
        }
    }

    bool Connection::ConnectServer()
    {
        uint32_t count = 0;

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
        {
            DEBUG_LOG("Server connected!");
            return true;
        }
    }
}
