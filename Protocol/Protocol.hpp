#pragma once

#include <stdint.h>

namespace CO2
{
    constexpr uint16_t SERVER_PORT = 16546;

    struct SensorData
    {
        uint32_t Timestamp;
        uint32_t Temperature;
        uint32_t Humidity;
        uint32_t CO2PPM;
    } __attribute__((packed));
}
