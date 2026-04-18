#include "DataParser.hpp"
#include "Utilities/Utilities.hpp"

#include <fmt/format.h>

#include <unordered_set>

namespace CO2::PC
{
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

    DataParser::ParsingResult DataParser::Parse(const std::string& message)
    {
        const auto tokens = Utilities::SplitString(message, ':');

        if (tokens.size() != 2)
        {
            fmt::println("Parsing error! No ':' token.");
            return {true};
        }

        const auto& header = tokens[0];
        const auto& data = tokens[1];

        const auto tag = header.substr(0, 3);
        if (!VerifyTag(tag))
        {
            fmt::println("Parsing error! Invalid message tag '{}'.", tag);
            return {true};
        }

        size_t dataLength = 0;
        try {
            dataLength = std::stoull(header.substr(3));
        }
        catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse data length '{}'.", e.what());
            return {true};
        }

        if (data.length() != dataLength)
        {
            fmt::println("Parsing error! Data length specified in header does not match actual data length.");
            return {true};
        }

        const auto readingTokens = Utilities::SplitString(data, ',');

        if (readingTokens.size() != 4)
        {
            fmt::println("Parsing error! Tokens count isn't equal to 4.");
            return {true};
        }

        SensorData sensorData;

        try
        {
            sensorData.Timestamp = std::stoul(readingTokens[0]);
            sensorData.Temperature = std::stof(readingTokens[1]);
            sensorData.Humidity = std::stof(readingTokens[2]);
            sensorData.CO2PPM = std::stoul(readingTokens[3]);
        }
        catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse a token '{}'.", e.what());
            return {true};
        }

        return {false, sensorData, tag};
    }

    DataParser::ParsingResult DataParser::ParseFileRow(const std::string& row)
    {

        const auto readingTokens = Utilities::SplitString(row, ',');

        if (readingTokens.size() != 4)
        {
            fmt::println("Parsing error! Tokens count isn't equal to 4.");
            return {true};
        }

        SensorData sensorData;

        try
        {
            sensorData.Timestamp = std::stoul(readingTokens[0]);
            sensorData.Temperature = std::stof(readingTokens[1]);
            sensorData.Humidity = std::stof(readingTokens[2]);
            sensorData.CO2PPM = std::stoul(readingTokens[3]);
        }
        catch (const std::exception& e)
        {
            fmt::println("Parsing error! Failed to parse a token '{}'.", e.what());
            return {true};
        }

        return {false, sensorData, ""};
    }

}
