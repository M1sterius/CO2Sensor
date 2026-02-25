#include "Sensor.hpp"

namespace CO2::Firmware
{
    Sensor::Sensor() = default;
    Sensor::~Sensor() = default;

    bool Sensor::Begin()
    {
        // TODO: Begin sensors. If any sensor fails, indicate a catastrophic error
        return true;
    }

    bool Sensor::Okay()
    {
        return false;
    }
}