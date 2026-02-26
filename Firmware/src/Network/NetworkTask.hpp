#pragma once

#include <Arduino.h>

#include "Connection.hpp"

namespace CO2::Firmware
{
    class NetworkTask
    {
    public:
        NetworkTask();
        ~NetworkTask();

        void Begin(QueueHandle_t hQueue, Connection* pConnection);
    private:
        QueueHandle_t m_hQueue;
        Connection* m_pConnection;

        static void TaskEntry(void* args);
        void Task();
    };

}
