#include "Utilities.hpp"

namespace CO2
{
    std::vector<std::string> Utilities::SplitString(const std::string& string, const char delimiter)
    {
        std::vector<std::string> tokens;

        size_t pos = 0;
        size_t oldPos = 0;
        while ((pos = string.find(delimiter, oldPos)) != std::string::npos)
        {
            const auto token = string.substr(oldPos, pos - oldPos);
            tokens.push_back(token);
            oldPos = pos + 1;
        }
        tokens.push_back(string.substr(oldPos, pos - oldPos));

        return tokens;
    }

    std::string Utilities::CurrentDate()
    {
        const auto today = std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
        );
        return std::format("{:%F}", std::chrono::year_month_day{today});
    }

    std::chrono::year_month_day Utilities::ParseDate(const std::string& date)
    {
        int y, m, d;
        char dash1, dash2;

        std::stringstream ss(date);
        ss >> y >> dash1 >> m >> dash2 >> d;

        return std::chrono::year{y}/m/d;
    }
}
