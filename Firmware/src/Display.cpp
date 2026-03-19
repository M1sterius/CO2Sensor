#include "Display.hpp"

namespace CO2::Firmware
{
    constexpr uint8_t LCD_I2C_ADDRESS = 0x27;
    constexpr uint8_t LCD_COLUMNS = 16;
    constexpr uint8_t LCD_ROWS = 2;

    Display::Display()
        : m_LCD(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS) { }
    Display::~Display() = default;

    void Display::Begin()
    {
        m_LCD.init();
        m_LCD.backlight();
    }

    void Display::ClearPrintf(const char* fmt, ...)
    {
        static char buffer[64];

        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);

        m_LCD.clear();
        m_LCD.print(buffer);
    }

    void Display::DrawDashboard(const float ppm, const float temp, const float humidity)
    {
        m_LCD.clear();
        m_LCD.setCursor(0, 0);
    }
}