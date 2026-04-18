#pragma once

#include "Protocol.hpp"

#include <fstream>
#include <unordered_set>
#include <string>
#include <filesystem>

namespace CO2::PC
{
    class DataSaver
    {
    public:
        DataSaver();
        ~DataSaver();

        void SaveReading(const SensorData& sensorData);

        std::unordered_set<std::string> QueryAvailableDates();
        size_t QueryTotalSizeOfFiles();
        void LoadDateReadings(const std::string& date);
    private:
        std::fstream m_CurrentFile;
        std::string m_CurrentDate;
        uint32_t m_CurrentCount;

        static std::filesystem::path DataPath();
        static bool VerifyFilename(const std::filesystem::path& path);
    };
}
