#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace CO2::Firmware
{
    class Display
    {
    public:
        Display();
        ~Display();

        void Begin();
        LiquidCrystal_I2C& Get() { return m_LCD; }
    
        void ClearPrintf(const char* fmt, ...);
        void DrawDashboard(const float ppm, const float temp, const float humidity);
    private:
        LiquidCrystal_I2C m_LCD;
    };
}