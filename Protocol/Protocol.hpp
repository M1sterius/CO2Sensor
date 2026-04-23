#pragma once

#include <stdio.h>
#include <stdint.h>

namespace CO2
{
    constexpr auto NTP_SERVER = "pool.ntp.org";

    // TODO: Move to config
    constexpr auto SERVER_MDNS = "co2-server.local";

    constexpr auto NEW_SENSOR_READING_TAG = "SRD";
    constexpr auto OLD_SENSOR_READING_TAG = "SRH";
    constexpr uint16_t SERVER_PORT = 16546;
    constexpr uint32_t SERIAL_BAUD = 115200;

    constexpr uint32_t SENSOR_READ_PERIOD = 7000;

    constexpr uint32_t SENSOR_QUEUE_SIZE = 16;
    constexpr uint32_t SENSOR_TASK_CORE_ID = 1;
    constexpr uint32_t NETWORK_TASK_CORE_ID = 0;

    struct SensorData
    {
        static_assert(sizeof(float) == 4, "float type must be 4 bytes wide!");

        uint32_t Timestamp{0};
        float Temperature{0};
        float Humidity{0};
        uint32_t CO2PPM{0};

        const char* ToString() const
        {
            static char buffer[128];
            snprintf(buffer, sizeof(buffer), "%u,%.2f,%.2f,%u", Timestamp, Temperature, Humidity, CO2PPM);
            return buffer;
        }
    };
}
