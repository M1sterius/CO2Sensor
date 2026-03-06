#pragma once

#include <Arduino.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>

namespace CO2::Firmware
{
    class Sensor
    {
    public:
        Sensor();
        ~Sensor();

        bool Begin();

        QueueHandle_t GetQueue() const { return m_Queue; }
    private:
        QueueHandle_t m_Queue;
        MQUnifiedsensor m_MQ135;
        DHT m_DHT;

        void InitMQ135();
        static void TaskEntry(void* args);
        void SensorTask();
    };
}