#pragma once

#include <Arduino.h>

namespace CO2 { struct SensorData; }

namespace CO2::Firmware
{
    class Connection;
    class DataSaver;

    class NetworkTask
    {
    public:
        NetworkTask();
        ~NetworkTask();

        void Begin(QueueHandle_t hQueue, Connection* pConnection, DataSaver* dataSaver);
    private:
        QueueHandle_t m_hQueue{nullptr};
        Connection* m_pConnection{nullptr};
        DataSaver* m_pDataSaver{nullptr};

        static void TaskEntry(void* args);
        void Task();

        const char* FormatDataString(const char* data);
    };

}
