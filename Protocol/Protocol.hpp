#pragma once

#include <stdio.h>
#include <stdint.h>

namespace CO2
{
    constexpr auto SERVER_MDNS = "co2-server.local";
    constexpr auto WIFI_SSID = "Glazgen2";
    constexpr auto WIFI_PASSWORD = "GlaZgeN0208";
    constexpr auto SERVER_IP = "192.168.1.102";
    constexpr auto SENSOR_DATA_STRING_TYPE = "SRD";
    constexpr uint16_t SERVER_PORT = 16546;
    
    constexpr uint32_t MAX_CONNECT_ATTEMPTS = 2;
    constexpr uint32_t CONNECT_ATTEMPT_DELAY = 50;
    constexpr uint32_t RECONNECT_ATTEMPT_DELAY = 10000;
    constexpr uint32_t SENSOR_READ_DELAY = 3000;
    constexpr uint32_t SENSOR_QUEUE_SIZE = 16;
    constexpr uint32_t SENSOR_TASK_CORE_ID = 0;
    constexpr uint32_t NETWORK_TASK_CORE_ID = 1;

    struct SensorData
    {
        static_assert(sizeof(float) == 4, "float type must be 4 bytes wide!");

        uint32_t Timestamp{0};
        float Temperature{0};
        float Humidity{0};
        float CO2PPM{0};

        inline const char* ToString()
        {
            static char buffer[128];
            snprintf(buffer, sizeof(buffer), "%u,%f,%f,%f", Timestamp, Temperature, Humidity, CO2PPM);
            return buffer;
        }
    };
}
