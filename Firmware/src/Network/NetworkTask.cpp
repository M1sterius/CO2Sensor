#include "NetworkTask.hpp"
#include "Connection.hpp"
#include "../DataSaver.hpp"
#include "Utilities/Debug.hpp"
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

                if (!m_pConnection->IsConnected())
                {
                    Serial.println(sensorData.ToString()); // TODO: Remove. For debug only
                    m_pDataSaver->Write(sensorData);

                    const auto now = xTaskGetTickCount();
                    if (now - lastReconnectAttempt > pdMS_TO_TICKS(RECONNECT_ATTEMPT_DELAY))
                    {
                        lastReconnectAttempt = xTaskGetTickCount();
                        m_pConnection->Reconnect();
                    }
                }
                else
                {
                    SensorData savedSensorData;

                    // Send readings gathered when there was no server connection
                    while (m_pDataSaver->Read(savedSensorData) && m_pConnection->IsConnected())
                        m_pConnection->Println(FormatDataString(savedSensorData.ToString(), OLD_SENSOR_READING_TAG));

                    // Send the latest reading
                    m_pConnection->Println(FormatDataString(sensorData.ToString(), NEW_SENSOR_READING_TAG));
                }
            }
        }
    }
    
    const char* NetworkTask::FormatDataString(const char* data, const char* tag)
    {
        static char buffer[128];
        snprintf(buffer, sizeof(buffer), "%s%u:%s", tag, strlen(data), data);
        return buffer;
    }
}
