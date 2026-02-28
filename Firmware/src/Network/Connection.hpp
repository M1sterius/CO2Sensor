#pragma once

#include <Arduino.h>
#include <WiFi.h>

namespace CO2::Firmware
{
    class Connection
    {
    public:
        Connection();
        ~Connection();

        bool Begin();
        void Terminate();
        void Reconnect();

        bool Connected();

        void Println(const char* str);
    private:
        WiFiClient m_WiFiClient;

        bool ConnectWiFi();
        bool ConnectServer();
    };
}