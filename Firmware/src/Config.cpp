#include "Config.hpp"
#include "Utilities/Debug.hpp"

namespace CO2::Firmware
{
    Config::Config() = default;
    Config::~Config() = default;

    bool Config::Begin()
    {
        const auto res = m_Prefs.begin("config", false);
        if (!res)
            return false;

        if (!m_Prefs.isKey("Configured") || !m_Prefs.isKey("WiFiSSID") ||
            !m_Prefs.isKey("WiFiPassword") || !m_Prefs.isKey("ServerIP"))
        {
            InitializeValues();
            m_IsConfigurationRequired = true;
            return true;
        }

        m_IsConfigurationRequired = !m_Prefs.getBool("Configured");
        m_Prefs.getString("WiFiSSID", m_WiFiSSID, (sizeof(m_WiFiSSID) / sizeof(char)) - 1);
        m_Prefs.getString("WiFiPassword", m_WiFiPassword, (sizeof(m_WiFiPassword) / sizeof(char)) - 1);
        m_Prefs.getString("ServerIP", m_ServerIP, (sizeof(m_ServerIP) / sizeof(char)) - 1);

        DEBUG_LOG("%s;%s;%s", m_WiFiSSID, m_WiFiPassword, m_ServerIP);

        m_Prefs.end();
        return true;
    }

    void Config::Configure()
    {
        while (true)
        {
            if (ReadSerialAndParse(m_WiFiSSID, m_WiFiPassword, m_ServerIP))
            {
                DEBUG_LOG("Config completed!");
                m_Prefs.begin("config", false);

                m_Prefs.putBool("Configured", true);
                m_Prefs.putString("WiFiSSID", m_WiFiSSID);
                m_Prefs.putString("WiFiPassword", m_WiFiPassword);
                m_Prefs.putString("ServerIP", m_ServerIP);

                m_IsConfigurationRequired = false;
                m_Prefs.end();

                return;
            }
        }
    }

    void Config::InitializeValues()
    {
        m_Prefs.putBool("Configured", false);
        m_Prefs.putString("WiFiSSID", "");
        m_Prefs.putString("WiFiPassword", "");
        m_Prefs.putString("ServerIP", "");
    }

    bool Config::ReadSerialAndParse(char* wifiSsid, char* wifiPassword, char* serverIp)
    {
        constexpr uint32_t BUF_LEN = 128;
        static char buffer[BUF_LEN];

        while (true) 
        {
            int idx = 0;

            // Read until newline
            while (true) 
            {
                while (!Serial.available())
                    delay(1);

                char c = Serial.read();

                // ignore CR
                if (c == '\r') 
                    continue; 

                if (c == '\n') 
                {
                    buffer[idx] = '\0';
                    break;
                }

                if (idx < BUF_LEN - 1)
                    buffer[idx++] = c;
                else 
                {
                    DEBUG_LOG("Config serial error: line too long!");
                    idx = 0; // reset and retry
                }
            }

            auto p1 = strtok(buffer, ";"); // WiFi SSID
            auto p2 = strtok(NULL, ";"); // WiFi password
            auto p3 = strtok(NULL, ";"); // server ip
            auto extra = strtok(NULL, ";");

            if (!p1 || !p2 || !p3 || extra) 
            {
                Serial.println("Config serial error: expected exactly 3 fields");
                continue; // retry
            }

            // TODO: Token length validation

            strcpy(wifiSsid, p1);
            strcpy(wifiPassword, p2);
            strcpy(serverIp, p3);

            return true;
        }
    }
}
