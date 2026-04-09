#pragma once

#include <Arduino.h>
#include <LittleFS.h>

namespace CO2 { struct SensorData; }

namespace CO2::Firmware
{
    class DataSaver
    {
    private:
        struct BufferState
        {
            uint32_t Head{0};
            uint32_t Tail{0};
            uint32_t Count{0};
        };

        struct BatchState
        {
            File BufferFile{nullptr};
            File ReadingsFile{nullptr};
            uint32_t Count{0};

            bool OpenFiles();
            void CloseFiles();
        };
    public:
        DataSaver();
        ~DataSaver();

        bool Begin();

        bool Read(SensorData& sensorData);
        void Write(const SensorData& data);
    private:
        static constexpr uint32_t CIRCULAR_BUFFER_SIZE = 57600;
        static constexpr uint32_t MAX_READINGS_PER_BATCH = 50;
        static constexpr auto SENSOR_READINGS_FILE = "/sensor/readings.dat";
        static constexpr auto SENSOR_BUFFER_FILE = "/sensor/buffer.dat";
        
        BatchState m_CurrentBatch;
    };
}
