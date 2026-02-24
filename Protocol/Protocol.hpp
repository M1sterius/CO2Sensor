#pragma once

#include <stdint.h>

namespace CO2
{
    constexpr auto SERVER_MDNS = "co2-server.local";
    constexpr auto WIFI_SSID = "Glazgen2";
    constexpr auto WIFI_PASSWORD = "GlaZgeN0208";
    constexpr auto SERVER_IP = "192.168.1.100";
    constexpr uint16_t SERVER_PORT = 16546;
    
    constexpr uint32_t MAX_CONNECT_ATTEMPTS = 5;
    constexpr uint32_t CONNECT_ATTEMPT_DELAY = 500;

    struct SensorData
    {
        uint32_t Timestamp;
        uint32_t Temperature;
        uint32_t Humidity;
        uint32_t CO2PPM;
    };
}
