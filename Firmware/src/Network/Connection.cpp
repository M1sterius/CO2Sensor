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

    bool Connection::IsConnected()
    {
        return WiFi.isConnected() && m_WiFiClient.connected();
    }

    bool Connection::IsTimeConfigured() const
    {
        return m_IsTimeConfigured;
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

        DEBUG_LOG("WiFi successfully connected!");
        DEBUG_LOG("Configuring time via NTP.");

        // TODO: move constants to a better place
        constexpr int MAX_NTP_RETRIES = 5;
        constexpr int NTP_TIMEOUT_MS = 5000;

        for (int attempt = 0; attempt < MAX_NTP_RETRIES; attempt++)
        {
            DEBUG_LOG("Sending NTP request...");
            configTime(0, 0, NTP_SERVER);

            auto start = millis();
            auto now = time(nullptr);

            while (now < 100000 && (millis() - start) < NTP_TIMEOUT_MS)
            {
                delay(100);
                now = time(nullptr);
            }

            if (now >= 100000)
            {
                DEBUG_LOG("Time successfully configured!");
                m_IsTimeConfigured = true;
                return true;
            }

            DEBUG_LOG("NTP sync failed, retrying...");
        }

        DEBUG_LOG("Failed to synchronize time after retries.");
        return false;
    }

    bool Connection::ConnectServer()
    {
        uint32_t count = 0;

        while (!m_WiFiClient.connect(SERVER_IP, SERVER_PORT) && count < MAX_CONNECT_ATTEMPTS)
        {
            DEBUG_LOG("Attempting to connect to the server.");
            m_WiFiClient.stop();
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
            DEBUG_LOG("Server successfully connected!");
            return true;
        }
    }
}
