#include "GraphDataProcessor.hpp"
#include "DataParser.hpp"
#include "Utilities/Utilities.hpp"

#include <filesystem>
#include <cassert>

namespace CO2::PC
{
    GraphDataProcessor::GraphDataProcessor() = default;
    GraphDataProcessor::~GraphDataProcessor() = default;

    bool GraphDataProcessor::LoadDateData(const std::string& date)
    {
        if (m_File.is_open())
            m_File.close();
        m_Storage.clear();

        const auto dataPath = std::filesystem::current_path() / "data";
        const auto filepath = std::filesystem::path((dataPath / date).string() + ".co2.txt");

        m_File = std::fstream(filepath, std::ios::in);
        if (!m_File.is_open())
            return false;

        std::string line;
        while (std::getline(m_File, line))
        {
            const auto [error, sensorData, _] = DataParser::ParseFileRow(line);
            if (!error)
                m_Storage.insert(sensorData);
        }

        return true;
    }

    GraphDataProcessor::GraphPointsContainer GraphDataProcessor::GetGraphPoints(const uint32_t readingId)
    {
        assert(readingId <= 2);

        GraphPointsContainer res;
        res.Points.reserve(m_Storage.size());

        for (const auto& sensorData : m_Storage)
        {
            const auto t = Utilities::TimestampToDayFraction(sensorData.Timestamp);
            double y = 0.0;

            switch (readingId)
            {
            case 0:
                y = static_cast<double>(sensorData.CO2PPM);
                res.Label = "Carbon Dioxide";
                break;
            case 1:
                y = static_cast<double>(sensorData.Temperature);
                res.Label = "Temperature";
                break;
            case 2:
                y = static_cast<double>(sensorData.Humidity);
                res.Label = "Humidity";
                break;
            default:
                y = static_cast<double>(sensorData.CO2PPM);
                res.Label = "Carbon Dioxide";
                break;
            }

            res.Points.push_back(QPointF(t, y));
            res.Min = std::min(res.Min, y);
            res.Max = std::max(res.Max, y);
        }

        return res;
    }
}
