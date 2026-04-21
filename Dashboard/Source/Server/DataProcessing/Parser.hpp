#pragma once

#include "Protocol.hpp"

#include <string>

namespace CO2::PC
{
    class Parser
    {
    public:
        struct ParsingResult
        {
            bool Error{false};
            SensorData SensorData{};
            std::string Tag{};
        };

        static ParsingResult Parse(const std::string& message);
        static ParsingResult ParseFileRow(const std::string& row);
    };
}
