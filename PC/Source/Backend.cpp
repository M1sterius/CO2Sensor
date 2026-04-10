#include "Backend.hpp"
#include "Server/Data/DataParser.hpp"

#include <fmt/format.h>

namespace CO2::PC
{
    Backend::Backend()
        : m_DataSaver(std::make_unique<DataSaver>())
    {
        m_Server = Server::Make([this](const std::string& msg){
            this->OnNewMessageReceived(msg);
        });
    }

    Backend::~Backend() = default;

    void Backend::Run()
    {
        m_Server->Run();
    }

    void Backend::OnNewMessageReceived(const std::string& message)
    {
        const auto [error, sensorData, tag] = DataParser::Parse(message);

        if (error)
            return;

        if (tag == NEW_SENSOR_READING_TAG)
        {
            m_Co2Avg.Push(sensorData.CO2PPM);
            m_TempAvg.Push(sensorData.Temperature);
            m_HumAvg.Push(sensorData.Humidity);

            fmt::println("CO2: {} PPM, T: {:.1f} C, H: {:.1f}", m_Co2Avg.GetAvg(), m_TempAvg.GetAvg(), m_HumAvg.GetAvg());
            // TODO: emit qml signal to update dashboard
        }

        m_DataSaver->SaveReading(sensorData);
    }
}
