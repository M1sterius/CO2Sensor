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
    private:
        Adafruit_BMP085 m_Barometer;
    };
}