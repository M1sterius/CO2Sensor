#pragma once

#include <serial/serial.h>

namespace CO2::PC
{
    class SerialHandler
    {
    public:
        static bool IsSensorConnected();
    };
}
