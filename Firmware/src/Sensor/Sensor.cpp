#include "Sensor.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    Sensor::Sensor() = default;
    Sensor::~Sensor() = default;

    bool Sensor::Begin()
    {
        m_Queue = xQueueCreate(SENSOR_QUEUE_SIZE, sizeof(SensorData));

        if (!m_Queue)
            return false;

        // TODO: Begin sensors. If any sensor fails, indicate a catastrophic error
        return true;
    }

    bool Sensor::Okay()
    {
        return false;
    }
}