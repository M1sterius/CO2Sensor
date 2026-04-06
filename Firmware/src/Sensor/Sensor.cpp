#include "Sensor.hpp"
#include "Utilities/Debug.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    Sensor::Sensor() = default;
    Sensor::~Sensor() = default;

    bool Sensor::Begin()
    {
        m_SCD40.begin(Wire, SCD40_I2C_ADDR_62);
        m_SCD40.stopPeriodicMeasurement();
        delay(300);
        m_SCD40.startPeriodicMeasurement();

        m_Queue = xQueueCreate(SENSOR_QUEUE_SIZE, sizeof(SensorData));

        if (!m_Queue)
        {
            DEBUG_LOG("Failed to create sensor data queue for network task!");
            return false;
        }

        return true;
    }

    void Sensor::BeginTask()
    {
        xTaskCreatePinnedToCore(&Sensor::TaskEntry, "SensorTask", 4096, this, 1, nullptr, SENSOR_TASK_CORE_ID);
    }

    void Sensor::GetDisplayStats(float& temp, float& hum, float& co2)
    {
        temp = m_TempAvg.GetAvg();
        hum = m_HumAvg.GetAvg();
        co2 = m_CO2Avg.GetAvg();
    }

    void Sensor::TaskEntry(void *args)
    {
        auto self = static_cast<Sensor*>(args);
        self->SensorTask();
    }

    void Sensor::SensorTask()
    {
        const auto period = pdMS_TO_TICKS(SENSOR_READ_DELAY);
        auto lastWakeTime = xTaskGetTickCount();

        while (true)
        {
            uint16_t co2;
            float temperature;
            float humidity;

            const auto error = m_SCD40.readMeasurement(co2, temperature, humidity);
            if (error)
                DEBUG_LOG("SCD 40 error!");

            SensorData sensorData;
            sensorData.Timestamp = time(nullptr);
            sensorData.Temperature = temperature;
            sensorData.Humidity = humidity;
            sensorData.CO2PPM = static_cast<float>(co2);

            m_TempAvg.Push(temperature);
            m_HumAvg.Push(humidity);
            m_CO2Avg.Push(static_cast<float>(co2));

            xQueueSend(m_Queue, &sensorData, portMAX_DELAY);
            vTaskDelayUntil(&lastWakeTime, period);
        }
    }
}
