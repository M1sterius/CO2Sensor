#include "DataHandler.hpp"
#include "Utilities/Utilities.hpp"

#include "fmt/format.h"

namespace CO2::PC
{
    static bool VerifyRange(const float value, const float min, const float max)
    {
        if (value == 0.0)
            return false;
        if (value < min || value > max)
            return false;
        return true;
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

        // Only one message type is currently supported
        const auto messageType = header.substr(0, 3);
        if (messageType != SENSOR_DATA_STRING_TYPE)
        {
            fmt::println("Parsing error! Invalid message type '{}'.", messageType);
            return;
        }

        size_t dataLength = 0;
        try {
            dataLength = std::stoull(header.substr(3));
        } catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse data length '{}'.", e.what());
            return;
        }

        if (data.length() != dataLength)
        {
            fmt::println("Parsing error! Data length specified in header does not match actual data length.");
            return;
        }

        const auto readings = Utilities::SplitString(data, ',');

        if (readings.size() != 4)
        {
            fmt::println("Parsing error! Readings count isn't equal to 4.");
            return;
        }

        SensorData sensorData;

        try
        {
            sensorData.Timestamp = std::stoul(readings[0]);
            sensorData.Temperature = std::stof(readings[1]);
            sensorData.Humidity = std::stof(readings[2]);
            sensorData.CO2PPM = std::stof(readings[3]);
        }
        catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse one of data reading '{}'.", e.what());
            return;
        }

        ProcessSensorData(sensorData);
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
}
