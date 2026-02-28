#include "NetworkTask.hpp"
#include "Connection.hpp"
#include "../Debug.hpp"
#include "../../Protocol/Protocol.hpp"

#include <LittleFS.h>

namespace CO2::Firmware
{
    NetworkTask::NetworkTask() = default;
    NetworkTask::~NetworkTask() = default;

    void NetworkTask::Begin(QueueHandle_t hQueue, Connection* pConnection)
    {
        m_hQueue = hQueue;
        m_pConnection = pConnection;

        xTaskCreatePinnedToCore(&NetworkTask::TaskEntry, "NetworkTask", 4096, this, 1, nullptr, NETWORK_TASK_CORE_ID);
    }

    void NetworkTask::TaskEntry(void* args)
    {
        auto self = static_cast<NetworkTask*>(args);
        self->Task();
    }

    void NetworkTask::Task()
    {
        TestData sensorData;

        while (true)
        {
            if (xQueueReceive(m_hQueue, &sensorData, portMAX_DELAY))
            {
                static int count = 0;

                if (!m_pConnection->Connected())
                {
                    auto str = String("No connection. Saving to flash!") + String(count);

                    DEBUG_LOG(str.c_str());
                    m_pConnection->Reconnect();

                    // Firstly, save data into flash to avoid queue overflow
                    // then try to reconnect
                }
                else
                {
                    auto str = String("Data received ") + String(count);

                    DEBUG_LOG(str.c_str());
                    m_pConnection->Println(str.c_str());

                    // Check if there's any data in flash that needs to be sent to the server
                    // Send it first if there's any
                    // Then send current data from the queue
                }

                count++;
            }
        }
    }
}
