#pragma once

#include "Config.hpp"

#include <Arduino.h>
#include <WiFi.h>

namespace CO2::Firmware
{
    class Connection
    {
    public:
        static constexpr uint32_t MAX_WIFI_CONNECT_ATTEMPTS = 5;
        static constexpr uint32_t MAX_SERVER_CONNECT_ATTEMPTS = 2;

        // How many ms to wait before attempting to reconnect during setup
        static constexpr uint32_t INITIAL_RECONNECT_ATTEMPT_DELAY = 500;

        // How many ms have to pass between each reconnect attempt when the device is already functioning
        static constexpr uint32_t RECONNECT_ATTEMPT_DELAY = 10000;

        Connection();
        ~Connection();

        bool Begin();
        void Terminate();
        void Reconnect();

        bool IsConnected();
        bool IsTimeConfigured() const;

        void Print(const char* str);
        void Println(const char* str);

        void SetConfigInstance(Config* pConfig);
    private:
        WiFiClient m_WiFiClient;
        bool m_IsTimeConfigured{false};
        Config* m_pConfig{nullptr};

        bool ConnectWiFi();
        bool ConnectServer();
    };
}