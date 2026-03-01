#include "Sensor.hpp"
#include "../Debug.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    Sensor::Sensor() = default;
    Sensor::~Sensor() = default;

    bool Sensor::Begin()
    {
        if (!m_Barometer.begin())
        {
            DEBUG_LOG("Failed to initialize barometer sensor!");
            return false;
        }

        xTaskCreatePinnedToCore(&Sensor::TaskEntry, "SensorTask", 4096, this, 1, nullptr, SENSOR_TASK_CORE_ID);
        m_Queue = xQueueCreate(SENSOR_QUEUE_SIZE, sizeof(SensorData));

        if (!m_Queue)
            return false;

        // TODO: Begin sensors. If any sensor fails, indicate a catastrophic error
        return true;
    }

    void Sensor::TaskEntry(void* args)
    {
        auto self = static_cast<Sensor*>(args);
        self->SensorTask();
    }

    void Sensor::SensorTask()
    {
        const auto period = pdMS_TO_TICKS(1000);
        auto lastWakeTime = xTaskGetTickCount();

        SensorData sensorData;

        while (true)
        {
            sensorData.Temperature = static_cast<uint32_t>(m_Barometer.readTemperature() * 100.0f);
            sensorData.Humidity = static_cast<uint32_t>(m_Barometer.readPressure());

            xQueueSend(m_Queue, &sensorData, portMAX_DELAY);
            vTaskDelayUntil(&lastWakeTime, period);
        }
    }
}
