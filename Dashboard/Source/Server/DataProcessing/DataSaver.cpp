#include "DataSaver.hpp"
#include "Utilities/Utilities.hpp"

#include <fmt/format.h>

#include <regex>
#include <filesystem>

namespace CO2::PC
{
    DataSaver::DataSaver()
        : m_CurrentCount(0)
    {
        const auto currentPath = std::filesystem::current_path();

        if (!std::filesystem::exists(currentPath / "data"))
            std::filesystem::create_directory(currentPath / "data");
    }

    DataSaver::~DataSaver() = default;

    void DataSaver::SaveReading(const SensorData& sensorData)
    {
        constexpr uint32_t BATCH_FLUSH_COUNT = 10;

        if (const auto date = Utilities::TimestampToDate(sensorData.Timestamp); date != m_CurrentDate)
        {
            if (m_CurrentFile.is_open())
                m_CurrentFile.close();

            m_CurrentCount = 0;
            m_CurrentFile = std::fstream(fmt::format("{}/data/{}.co2.txt",
                std::filesystem::current_path().string(), date), std::ios::app);

            if (!m_CurrentFile.is_open())
            {
                fmt::println("Failed to open file to save sensor reading.!");
                return;
            }

            m_CurrentDate = date;
        }

        m_CurrentFile << sensorData.ToString() << '\n';
        m_CurrentCount++;

        if (m_CurrentCount >= BATCH_FLUSH_COUNT)
        {
            m_CurrentFile.flush();
            m_CurrentCount = 0;
        }
    }

    std::unordered_set<std::string> DataSaver::QueryAvailableDates()
    {
        std::unordered_set<std::string> res;

        for (const auto& file : std::filesystem::directory_iterator(DataPath()))
        {
            const auto filename = file.path().filename();

            if (VerifyFilename(filename))
                res.insert(filename.stem().stem().string());
        }

        return res;
    }

    size_t DataSaver::QueryTotalSizeOfFiles()
    {
        size_t res{0};
        for (const auto& file : std::filesystem::directory_iterator(DataPath()))
        {
            const auto filename = file.path().filename();

            if (VerifyFilename(filename))
                res += std::filesystem::file_size(file.path());
        }

        return res;
    }

    void DataSaver::LoadDateReadings(const std::string &date)
    {

    }

    std::filesystem::path DataSaver::DataPath()
    {
        return std::filesystem::current_path() / "data";
    }

    bool DataSaver::VerifyFilename(const std::filesystem::path& path)
    {
        const auto pattern = std::regex(R"(^\d{4}-\d{2}-\d{2}\.co2\.txt$)");
        return std::regex_match(path.string(), pattern);
    }
}
