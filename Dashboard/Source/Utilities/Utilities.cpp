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

    std::string Utilities::TimestampToDate(const time_t timestamp)
    {
        tm* ti = localtime(&timestamp);

        char date[11];
        strftime(date, sizeof(date), "%Y-%m-%d", ti);

        return date;
    }

    std::string Utilities::TimestampToLastReadingTime(const time_t timestamp)
    {
        std::tm localTime{};

        #if defined(_WIN32)
            localtime_s(&localTime, &timestamp);
        #else
            localtime_r(&timestamp, &localTime);
        #endif

        std::ostringstream oss;
        oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");

        return oss.str();
    }

    double Utilities::TimestampToDayFraction(const time_t timestamp)
    {
        std::tm localTime{};

        #if defined(_WIN32)
                localtime_s(&localTime, &timestamp);
        #else
                localtime_r(&timestamp, &localTime);
        #endif

        double hours = static_cast<double>(localTime.tm_hour);
        double minutes = static_cast<double>(localTime.tm_min) / 60.0;
        double seconds = static_cast<double>(localTime.tm_sec) / 3600.0;

        return hours + minutes + seconds; // range: [0, 24)
    }
}
