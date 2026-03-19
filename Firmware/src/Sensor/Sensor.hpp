#pragma once

#include "Utilities/RunningAvgHelper.hpp"

#include <Arduino.h>
#include <MQUnifiedsensor.h>
#include <DHT.h>

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
        MQUnifiedsensor m_MQ135;
        DHT m_DHT;

        void InitMQ135();
        static void TaskEntry(void* args);
        void SensorTask();
    };
}