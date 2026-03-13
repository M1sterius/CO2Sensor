#pragma once

#include "Protocol.hpp"
#include "Utilities/RunningAvgHelper.hpp"

#include <string>
#include <vector>
#include <list>

// <MESSAGE_TYPE - 3 letters><MESSAGE_LENGTH>:<DATA...>
// SRD12:34,123,23,12

namespace CO2::PC
{
    class DataHandler
    {
    public:
        DataHandler();
        ~DataHandler();

        void Parse(const std::string& message);
    private:
        uint32_t m_Readings = 0;
        uint32_t m_BadReadings = 0;
        RunningAvgHelper<float, 10> m_TempAvg;
        RunningAvgHelper<float, 10> m_HumidityAvg;
        RunningAvgHelper<float, 10> m_CO2Avg;

        void ProcessSensorData(const SensorData& sensorData);
    };
}
