#include "DataHandler.hpp"
#include "Utilities/Utilities.hpp"

#include <fmt/format.h>

#include <unordered_set>

namespace CO2::PC
{
    static bool VerifyRange(const float value, const float min, const float max)
    {
        if (isnan(value) || value < min || value > max)
            return false;
        return true;
    }

    static bool VerifyTag(const std::string& tag)
    {
        static std::unordered_set<std::string> allowedTags;
        if (allowedTags.empty())
        {
            allowedTags.insert(NEW_SENSOR_READING_TAG);
            allowedTags.insert(OLD_SENSOR_READING_TAG);
        }

        return allowedTags.contains(tag);
    }

    DataHandler::DataHandler() = default;
    DataHandler::~DataHandler() = default;

    void DataHandler::Parse(const std::string& message)
    {
        const auto tokens = Utilities::SplitString(message, ':');

        if (tokens.size() != 2)
        {
            fmt::println("Parsing error! No ':' token.");
            return;
        }

        const auto& header = tokens[0];
        const auto& data = tokens[1];

        const auto tag = header.substr(0, 3);
        if (!VerifyTag(tag))
        {
            fmt::println("Parsing error! Invalid message tag '{}'.", tag);
            return;
        }

        size_t dataLength = 0;
        try {
            dataLength = std::stoull(header.substr(3));
        }
        catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse data length '{}'.", e.what());
            return;
        }

        if (data.length() != dataLength)
        {
            fmt::println("Parsing error! Data length specified in header does not match actual data length.");
            return;
        }

        const auto readingTokens = Utilities::SplitString(data, ',');

        if (readingTokens.size() != 4)
        {
            fmt::println("Parsing error! Tokens count isn't equal to 4.");
            return;
        }

        SensorData sensorData;

        try
        {
            sensorData.Timestamp = std::stoul(readingTokens[0]);
            sensorData.Temperature = std::stof(readingTokens[1]);
            sensorData.Humidity = std::stof(readingTokens[2]);
            sensorData.CO2PPM = std::stof(readingTokens[3]);
        }
        catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse a token '{}'.", e.what());
            return;
        }

        if (tag == NEW_SENSOR_READING_TAG)
            ProcessNewReading(sensorData);
        else if (tag == OLD_SENSOR_READING_TAG)
            ProcessOldReading(sensorData);
    }

    void DataHandler::ProcessSensorData(const SensorData& sensorData)
    {
        m_Readings++;

        if (!VerifyRange(sensorData.Temperature, -20, 50) ||
            !VerifyRange(sensorData.Humidity, 1, 100) ||
            !VerifyRange(sensorData.CO2PPM, 0.1, 5000))
        {
            m_BadReadings++;
            return;
        }

        m_TempAvg.Push(sensorData.Temperature);
        m_HumidityAvg.Push(sensorData.Humidity);
        m_CO2Avg.Push(sensorData.CO2PPM);

        fmt::println("\r\r\rReadings: {}, Bad readings: {}.", m_Readings, m_BadReadings);
        fmt::println("Temp: {:.1f}, Hum: {:.1f}, CO2: {:.2f}.", m_TempAvg.GetAvg(), m_HumidityAvg.GetAvg(), m_CO2Avg.GetAvg());
    }

    void DataHandler::ProcessNewReading(const SensorData& sensorData)
    {
        const auto str = sensorData.ToString();
        fmt::println("NEW: {}", str);
    }

    void DataHandler::ProcessOldReading(const SensorData& sensorData)
    {
        const auto str = sensorData.ToString();
        fmt::println("OLD: {}", str);
    }
}
