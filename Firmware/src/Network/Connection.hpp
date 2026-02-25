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

        bool Connected();
        bool WiFiConnected();
        bool ServerConnected();

        void BeginNetworkTask(QueueHandle_t queue);
    private:
        WiFiClient m_WiFiClient;
    };
}