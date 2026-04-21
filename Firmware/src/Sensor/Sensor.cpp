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

        int16_t error{0};
        error = m_SCD40.stopPeriodicMeasurement();

        if (error)
        {
            DEBUG_LOG("Failed to initialize SCD40 sensor. Error: %i.", error);
            return false;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
        error = m_SCD40.startPeriodicMeasurement();

        if (error)
        {
            DEBUG_LOG("Failed to initialize SCD40 sensor. Error: %i.", error);
            return false;
        }

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

    void Sensor::GetDisplayStats(float& temp, float& hum, uint32_t& co2)
    {
        temp = m_TempAvg.GetAvg();
        hum = m_HumAvg.GetAvg();
        co2 = m_CO2Avg.GetAvg();
    }

    void Sensor::TaskEntry(void* args)
    {
        auto self = static_cast<Sensor*>(args);
        self->SensorTask();
    }

    void Sensor::SensorTask()
    {
        constexpr auto SENSOR_PERIOD = pdMS_TO_TICKS(SENSOR_READ_PERIOD);

        auto lastWakeTime = xTaskGetTickCount();
        uint16_t co2{0};
        float temperature{0};
        float humidity{0};

        while (true)
        {
            int16_t error{0};
            bool ready{false};

            error = m_SCD40.getDataReadyStatus(ready);

            if (!error && ready)
            {
                error = m_SCD40.readMeasurement(co2, temperature, humidity);
                if (error)
                    DEBUG_LOG("Failed to read SCD40. Error: %i.", error);
                else
                {
                    SensorData sensorData{};
                    sensorData.Timestamp = time(nullptr);
                    sensorData.Temperature = temperature;
                    sensorData.Humidity = humidity;
                    sensorData.CO2PPM = static_cast<uint32_t>(co2);

                    m_TempAvg.Push(temperature);
                    m_HumAvg.Push(humidity);
                    m_CO2Avg.Push(static_cast<uint32_t>(co2));

                    xQueueSend(m_Queue, &sensorData, portMAX_DELAY);
                }
            }
            else
                DEBUG_LOG("SCD40 wasn't ready for data read. Error: %i.", error);

            vTaskDelayUntil(&lastWakeTime, SENSOR_PERIOD);
        }
    }
}
