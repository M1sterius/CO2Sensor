#pragma once

#include <cstdint>
#include <string>

namespace CO2::PC
{
    class RangesHelper
    {
    public:
        static uint32_t CalculateRangeIndex(const uint32_t ppm);

        static std::string GetRangeWord(const uint32_t index);
        static std::string GetRangeDescription(const uint32_t index);
    };
}