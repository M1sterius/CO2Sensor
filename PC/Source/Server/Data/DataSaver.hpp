#pragma once

#include "Protocol.hpp"

#include <fstream>

namespace CO2::PC
{
    class DataSaver
    {
    public:
        DataSaver();
        ~DataSaver();

        void SaveReading(const SensorData& sensorData);
    private:
        std::fstream m_CurrentFile;
        std::string m_CurrentDate;
        uint32_t m_CurrentCount;
    };
}