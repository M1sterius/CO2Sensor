#pragma once

#include <Arduino.h>
#include <Adafruit_BMP085.h>

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
        Adafruit_BMP085 m_Barometer;
        QueueHandle_t m_Queue;

        static void TaskEntry(void* args);
        void SensorTask();
    };
}