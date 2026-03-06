#include "Sensor.hpp"
#include "../Debug.hpp"
#include "../../Protocol/Protocol.hpp"


namespace CO2::Firmware
{
    constexpr auto MQ_BOARD_NAME = "ESP-32";
    constexpr auto MQ_VOLTAGE_RESOLUTION = 3.3;
    constexpr auto MQ_ADC_RESOLUTION = 12;
    constexpr auto MQ_SENSOR_PIN = 34;
    constexpr auto MQ_SENSOR_TYPE = "MQ-135";

    constexpr auto DHT_SENSOR_PIN = 4;
    constexpr auto DHT_TYPE = DHT11;

    Sensor::Sensor()
        : m_MQ135(MQ_BOARD_NAME, MQ_VOLTAGE_RESOLUTION, MQ_ADC_RESOLUTION, MQ_SENSOR_PIN, MQ_SENSOR_TYPE), 
          m_DHT(DHT_SENSOR_PIN, DHT_TYPE) { }
    Sensor::~Sensor() = default;

    bool Sensor::Begin()
    {
        InitMQ135();
        m_DHT.begin();

        xTaskCreatePinnedToCore(&Sensor::TaskEntry, "SensorTask", 4096, this, 1, nullptr, SENSOR_TASK_CORE_ID);
        m_Queue = xQueueCreate(SENSOR_QUEUE_SIZE, sizeof(SensorData));

        if (!m_Queue)
        {
            DEBUG_LOG("Failed to create sensor data queue for network task!");
            return false;
        }

        // TODO: Begin sensors. If any sensor fails, indicate a catastrophic error
        return true;
    }

    void Sensor::InitMQ135()
    {
        m_MQ135.setRegressionMethod(1);
        m_MQ135.setA(110.47); // Magic numbers for CO2 from 'MQUnifiedsensor by Miguel Califa' official examples
        m_MQ135.setB(-2.862);
        m_MQ135.init();
        m_MQ135.setR0(8.0);

        // Serial.print("Calibrating please wait.");
        // float calcR0 = 0;
        // for(int i = 1; i <= 10; i++)
        // {
        //     m_MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
        //     calcR0 += m_MQ135.calibrate(3.6);
        //     Serial.print(".");
        // }
        // m_MQ135.setR0(calcR0 / 10);
        // Serial.println("  done!.");
  
        // if(isinf(calcR0)) {Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
        // if(calcR0 == 0){Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}

    }

    void Sensor::TaskEntry(void *args)
    {
        auto self = static_cast<Sensor*>(args);
        self->SensorTask();
    }

    void Sensor::SensorTask()
    {
        const auto period = pdMS_TO_TICKS(1000);
        auto lastWakeTime = xTaskGetTickCount();

        while (true)
        {
            SensorData sensorData;

            const auto temperature = m_DHT.readTemperature();
            const auto humidity = m_DHT.readHumidity();

            if (isnan(temperature) || isnan(humidity))
                DEBUG_LOG("Failed to read DHT sensor");
            else
            {
                Serial.printf("T: %.2f, H: %.2f\n", temperature, humidity);
            }

            m_MQ135.update(); // Update data, the arduino will read the voltage from the analog pin
            const auto co2ppm = m_MQ135.readSensor();
            Serial.printf("CO2: %.2f\n", co2ppm);

            sensorData.Temperature = static_cast<uint32_t>(0);
            sensorData.Humidity = static_cast<uint32_t>(0);

            xQueueSend(m_Queue, &sensorData, portMAX_DELAY);
            vTaskDelayUntil(&lastWakeTime, period);
        }
    }
}
