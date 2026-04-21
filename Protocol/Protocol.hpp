#pragma once

#include <stdio.h>
#include <stdint.h>

namespace CO2
{
    constexpr auto SERVER_MDNS = "co2-server.local";
    constexpr auto NTP_SERVER = "pool.ntp.org";
    constexpr auto WIFI_SSID = "Glazgen2";
    constexpr auto WIFI_PASSWORD = "GlaZgeN0208";
    constexpr auto SERVER_IP = "192.168.1.102";
    constexpr auto NEW_SENSOR_READING_TAG = "SRD";
    constexpr auto OLD_SENSOR_READING_TAG = "SRH";
    constexpr uint16_t SERVER_PORT = 16546;
    constexpr uint32_t SERIAL_BAUD = 115200;
    
    // TODO: Separate wifi and server connect attampts and delays
    constexpr uint32_t MAX_CONNECT_ATTEMPTS = 2;
    constexpr uint32_t CONNECT_ATTEMPT_DELAY = 500;
    constexpr uint32_t RECONNECT_ATTEMPT_DELAY = 10000;
    constexpr uint32_t SENSOR_READ_DELAY = 5000;
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
            snprintf(buffer, sizeof(buffer), "%u,%f,%f,%u", Timestamp, Temperature, Humidity, CO2PPM);
            return buffer;
        }
    };
}
