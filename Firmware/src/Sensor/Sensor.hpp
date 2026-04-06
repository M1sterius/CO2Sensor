#pragma once

#include "Utilities/RunningAvgHelper.hpp"

#include <Arduino.h>
#include <SensirionI2CScd4x.h>

namespace CO2::Firmware
{
    class Sensor
    {
    public:
        Sensor();
        ~Sensor();

        bool Begin();
        void BeginTask();

        QueueHandle_t GetQueue() const { return m_Queue; }
        void GetDisplayStats(float& temp, float& hum, float& co2);
    private:
        RunningAvgHelper<float, 5> m_TempAvg;
        RunningAvgHelper<float, 5> m_HumAvg;
        RunningAvgHelper<float, 5> m_CO2Avg;

        QueueHandle_t m_Queue;
        SensirionI2cScd4x m_SCD40;

        static void TaskEntry(void* args);
        void SensorTask();
    };
}