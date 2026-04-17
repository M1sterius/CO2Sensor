#include "RangesHelper.hpp"

#include <cassert>

namespace CO2::PC
{
    uint32_t RangesHelper::CalculateRangeIndex(const uint32_t ppm)
    {
        if (ppm <= 450) // Excellent
            return 0;
        else if (ppm <= 600) // Good
            return 1;
        else if (ppm <= 900) // Normal
            return 2;
        else if (ppm <= 1200) // Poor
            return 3;
        else if (ppm <= 1500) // Bad
            return 4;
        else if (ppm <= 2000) // Awful
            return 5;
        else // Dangerous
            return 6;
    }

    std::string RangesHelper::GetRangeWord(const uint32_t index)
    {
        assert(index <= 6);

        switch (index)
        {
        case 0:
            return "Excellent";
        case 1:
            return "Good";
        case 2:
            return "Normal";
        case 3:
            return "Poor";
        case 4:
            return "Bad";
        case 5:
            return "Awful";
        default:
            return "Dangerous";
        }
    }

    std::string RangesHelper::GetRangeDescription(const uint32_t index)
    {
        assert(index <= 6);
        return "TODO"; // TODO: Implement
    }
}
