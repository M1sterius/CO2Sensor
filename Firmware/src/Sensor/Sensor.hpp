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

        bool Okay();

        QueueHandle_t GetQueue() const { return m_Queue; }
    private:
        QueueHandle_t m_Queue;
        Adafruit_BMP085 m_Barometer;
    };
}