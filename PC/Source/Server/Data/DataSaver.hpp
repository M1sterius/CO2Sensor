#pragma once

#include "Protocol.hpp"

namespace CO2::PC
{
    class DataSaver
    {
    public:
        DataSaver();
        ~DataSaver();

        void SaveReading(const SensorData& sensorData);
    private:

    };
}