#pragma once

#include <stdio.h>
#include <stdint.h>

namespace CO2
{
    constexpr auto SERVER_MDNS = "co2-server.local";
    constexpr auto WIFI_SSID = "Glazgen2";
    constexpr auto WIFI_PASSWORD = "GlaZgeN0208";
    constexpr auto SERVER_IP = "192.168.1.102";
    constexpr uint16_t SERVER_PORT = 16546;
    
    // TODO: Make constants better
    constexpr uint32_t MAX_CONNECT_ATTEMPTS = 5;
    constexpr uint32_t CONNECT_ATTEMPT_DELAY = 500;
    constexpr uint32_t RECONNECT_ATTEMPT_INTERVAL = 5000;
    constexpr uint32_t SENSOR_QUEUE_SIZE = 10;
    constexpr uint32_t SENSOR_TASK_CORE_ID = 0;
    constexpr uint32_t NETWORK_TASK_CORE_ID = 1;

    struct SensorData
    {
        uint32_t Timestamp{0};
        uint32_t Temperature{0};
        uint32_t Humidity{0};
        uint32_t CO2PPM{0};
    };
}
