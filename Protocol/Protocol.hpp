#pragma once

#include <stdio.h>
#include <stdint.h>

namespace CO2
{
    constexpr auto SERVER_MDNS = "co2-server.local";
    constexpr auto WIFI_SSID = "Glazgen2";
    constexpr auto WIFI_PASSWORD = "GlaZgeN0208";
    constexpr auto SERVER_IP = "192.168.1.101";
    constexpr auto SENSOR_DATA_STRING_TYPE = "SRD";
    constexpr uint16_t SERVER_PORT = 16546;
    
    // TODO: Make constants better
    constexpr uint32_t MAX_CONNECT_ATTEMPTS = 3;
    constexpr uint32_t CONNECT_ATTEMPT_DELAY = 500;
    constexpr uint32_t RECONNECT_ATTEMPT_INTERVAL = 10000;
    constexpr uint32_t MAX_SENSOR_INTERVAL = 5000;
    constexpr uint32_t SENSOR_QUEUE_SIZE = 10;
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
