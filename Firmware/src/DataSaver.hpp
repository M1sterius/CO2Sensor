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
        static constexpr uint32_t CIRCULAR_BUFFER_SIZE = 57600;
        static constexpr auto SENSOR_READINGS_FILE = "/sensor/readings.dat";
        static constexpr auto SENSOR_BUFFER_FILE = "/sensor/buffer.dat";

        uint32_t GetWritePos();
        void UpdateCircularBuffer();
    };
}
