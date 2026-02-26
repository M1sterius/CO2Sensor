#include "NetworkTask.hpp"
#include "../Debug.hpp"

namespace CO2::Firmware
{
    NetworkTask::NetworkTask() = default;
    NetworkTask::~NetworkTask() = default;

    void NetworkTask::Begin(QueueHandle_t hQueue, Connection* pConnection)
    {
        m_hQueue = hQueue;
        m_pConnection = pConnection;

        xTaskCreate(&NetworkTask::TaskEntry, "NetworkTask", 4096, this, 1, NULL);
    }

    void NetworkTask::TaskEntry(void* args)
    {
        auto self = static_cast<NetworkTask*>(args);
        self->Task();
    }

    void NetworkTask::Task()
    {
        const auto period = pdMS_TO_TICKS(1000);
        auto lastWakeTime = xTaskGetTickCount();

        while (true)
        {
            if (m_pConnection->Connected())
                DEBUG_LOG("Network task!");
            else
            {
                DEBUG_LOG("No connection!");
                m_pConnection->Begin();
            }

            vTaskDelayUntil(&lastWakeTime, period);
        }
        
    }
}