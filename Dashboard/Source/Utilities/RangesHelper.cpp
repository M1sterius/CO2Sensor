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

    std::string RangesHelper::GetRangeDescription(const uint32_t ppm)
    {
        const auto index = CalculateRangeIndex(ppm);
        assert(index <= 6);

        switch (index)
        {
        case 0: // <= 450
            return "Air quality is excellent with fresh air, supporting optimal comfort and concentration.";
        case 1: // <= 600
            return "Air quality is good and comfortable with no noticeable effects on occupants.";
        case 2: // <= 900
            return "Air quality is acceptable but may start to feel slightly stale in enclosed spaces.";
        case 3: // <= 1200
            return "Air quality is poor and may cause mild drowsiness or reduced concentration.";
        case 4: // <= 1500
            return "Air quality is bad and can lead to noticeable sleepiness, headaches, and decreased focus.";
        case 5: // <= 2000
            return "Air quality is very bad and may cause fatigue, headaches, and impaired cognitive performance.";
        case 6: // > 2000
            return "Air quality is dangerous and prolonged exposure can cause significant drowsiness and discomfort.";
        default:
            return "Unknown air quality level.";
        }
    }
}
