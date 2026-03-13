#pragma once

#include <Arduino.h>
#include <LittleFS.h>

namespace CO2 { struct SensorData; }

namespace CO2::Firmware
{
    class DataSaver
    {
    public:
        DataSaver();
        ~DataSaver();

        bool Begin();

        bool Read(char* buffer, const uint32_t bufferSize);
        void Write(const SensorData& data);
    private:
        File m_File;
    };
}
