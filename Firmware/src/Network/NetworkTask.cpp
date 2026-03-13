#include "NetworkTask.hpp"
#include "Connection.hpp"
#include "../DataSaver.hpp"
#include "../Debug.hpp"
#include "../../Protocol/Protocol.hpp"

#include <LittleFS.h>

namespace CO2::Firmware
{
    NetworkTask::NetworkTask() = default;
    NetworkTask::~NetworkTask() = default;

    void NetworkTask::Begin(QueueHandle_t hQueue, Connection* pConnection, DataSaver* dataSaver)
    {
        m_hQueue = hQueue;
        m_pConnection = pConnection;
        m_pDataSaver = dataSaver;

        xTaskCreatePinnedToCore(&NetworkTask::TaskEntry, "NetworkTask", 4096, this, 1, nullptr, NETWORK_TASK_CORE_ID);
    }

    void NetworkTask::TaskEntry(void* args)
    {
        auto self = static_cast<NetworkTask*>(args);
        self->Task();
    }

    void NetworkTask::Task()
    {
        SensorData sensorData;

        while (true)
        {
            if (xQueueReceive(m_hQueue, &sensorData, portMAX_DELAY))
            {
                static TickType_t lastReconnectAttempt = 0;

                if (!m_pConnection->Connected())
                {
                    Serial.println(sensorData.ToString());
                    m_pDataSaver->Write(sensorData);

                    const auto now = xTaskGetTickCount();
                    if (now - lastReconnectAttempt > pdMS_TO_TICKS(RECONNECT_ATTEMPT_INTERVAL))
                    {
                        lastReconnectAttempt = xTaskGetTickCount();
                        m_pConnection->Reconnect();
                    }
                }
                else
                {
                    static char buffer[128];

                    while (m_pDataSaver->Read(buffer, sizeof(buffer)) && m_pConnection->Connected())
                        m_pConnection->Println(FormatDataString((buffer)));

                    m_pConnection->Println(FormatDataString(sensorData.ToString()));
                }
            }
        }
    }
    
    const char* NetworkTask::FormatDataString(const char* data)
    {
        static char buffer[128];
        snprintf(buffer, sizeof(buffer), "%s%u:%s", SENSOR_DATA_STRING_TYPE, strlen(data), data);
        return buffer;
    }
}
