#pragma once

#include <Arduino.h>
#include <Preferences.h>

namespace CO2::Firmware
{
    class Config
    {
    public:
        Config();
        ~Config();

        bool Begin();
        void Configure();

        bool IsConfigurationRequired() const { return m_IsConfigurationRequired; }
        const char* GetWiFiSSID() const { return m_WiFiSSID; }
        const char* GetWiFiPassword() const { return m_WiFiPassword; }
        const char* GetServerIP() const { return m_ServerIP; }
    private:
        Preferences m_Prefs;

        bool m_IsConfigurationRequired{true};
        char m_WiFiSSID[33];
        char m_WiFiPassword[64];
        char m_ServerIP[16];

        void InitializeValues();
        bool ReadSerialAndParse(char* wifiSsid, char* wifiPassword, char* serverIp);
    };
}
