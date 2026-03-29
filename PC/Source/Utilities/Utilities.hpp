#pragma once

#include <string>
#include <vector>
#include <chrono>

namespace CO2
{
    class Utilities
    {
    public:
        static std::vector<std::string> SplitString(const std::string& string, const char delimiter = ' ');

        static std::string CurrentDate();
        static std::chrono::year_month_day ParseDate(const std::string& date);

        static std::string TimestampToDate(const time_t timestamp);
    };
}