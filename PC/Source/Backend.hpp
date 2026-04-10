#pragma once

#include "Server/Server.hpp"
#include "Server/Data/DataSaver.hpp"
#include "Utilities/RunningAvgHelper.hpp"

#include <memory>

namespace CO2::PC
{
    class Backend
    {
    public:
        Backend();
        ~Backend();

        void Run();
    private:
        std::shared_ptr<Server> m_Server;
        std::unique_ptr<DataSaver> m_DataSaver;
        RunningAvgHelper<uint32_t, 10> m_Co2Avg;
        RunningAvgHelper<float, 10> m_TempAvg;
        RunningAvgHelper<float, 10> m_HumAvg;

        void OnNewMessageReceived(const std::string& message);
    };
}