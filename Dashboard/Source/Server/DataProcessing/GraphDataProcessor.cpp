#include "GraphDataProcessor.hpp"
#include "Parser.hpp"
#include "Utilities/Utilities.hpp"
#include "ReadingType.hpp"

#include <fmt/format.h>

#include <filesystem>
#include <cassert>
#include <regex>

namespace CO2::PC
{
    static std::filesystem::path DataPath()
    {
        return std::filesystem::current_path() / "data";
    }

    static std::filesystem::path FormatPath(const std::string& date)
    {
        return std::filesystem::path((DataPath() / date).string() + ".co2.txt");
    }

    static bool VerifyFilename(const std::filesystem::path& filename)
    {
        const auto pattern = std::regex(R"(^\d{4}-\d{2}-\d{2}\.co2\.txt$)");
        return std::regex_match(filename.string(), pattern);
    }

    GraphDataProcessor::GraphDataProcessor() = default;
    GraphDataProcessor::~GraphDataProcessor() = default;

    // Runs on the main thread
    bool GraphDataProcessor::LoadDateReadings(const std::string& date)
    {
        m_Storage.clear();

        std::unique_lock lock(m_Mutex);

        // If we are reading the active file, force flush so we don't miss the last 9 readings
        if (date == m_SaveState.Date && m_SaveState.File.is_open())
            m_SaveState.File.flush();

        auto file = std::fstream(FormatPath(date), std::ios::in);
        if (!file.is_open())
            return false;

        std::string line;
        while (std::getline(file, line))
        {
            const auto [error, sensorData, _] = Parser::ParseFileRow(line);
            if (!error)
                m_Storage.insert(sensorData);
        }

        return true;
    }

    // Runs on event (server) thread
    void GraphDataProcessor::SaveReading(const SensorData& sensorData)
    {
        constexpr uint32_t BATCH_FLUSH_COUNT = 10;

        std::unique_lock lock(m_Mutex);

        if (const auto date = Utilities::TimestampToDate(sensorData.Timestamp); date != m_SaveState.Date)
        {
            if (m_SaveState.File.is_open())
                m_SaveState.File.close();

            m_SaveState.Count = 0;
            m_SaveState.File = std::fstream(FormatPath(date), std::ios::app);

            if (!m_SaveState.File.is_open())
            {
                fmt::println("Failed to open file to save sensor reading.!");
                return;
            }

            m_SaveState.Date = date;
        }

        m_SaveState.File << sensorData.ToString() << '\n';
        m_SaveState.Count++;

        if (m_SaveState.Count >= BATCH_FLUSH_COUNT)
        {
            m_SaveState.File.flush();
            m_SaveState.Count = 0;
        }
    }

    QVariantList GraphDataProcessor::QueryAvailableDates() const
    {
        QVariantList dates;

        for (const auto& file : std::filesystem::directory_iterator(DataPath()))
        {
            const auto filename = file.path().filename();

            if (VerifyFilename(filename))
                dates.append(QString::fromStdString(filename.stem().stem().string()));
        }

        return dates;
    }

    size_t GraphDataProcessor::QueryTotalSizeOfFiles() const
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

    GraphDataProcessor::GraphPointsContainer GraphDataProcessor::GetGraphPoints(const uint32_t readingId)
    {
        assert(readingId <= 2);

        GraphPointsContainer res;

        for (const auto& sensorData : m_Storage)
        {
            const auto t = Utilities::TimestampToDayFraction(sensorData.Timestamp);
            double y = 0.0;

            switch (static_cast<ReadingType>(readingId))
            {
            case ReadingType::CarbonDioxide:
                y = static_cast<double>(sensorData.CO2PPM);
                res.Label = "Carbon Dioxide";
                break;
            case ReadingType::Temperature:
                y = static_cast<double>(sensorData.Temperature);
                res.Label = "Temperature";
                break;
            case ReadingType::Humidity:
                y = static_cast<double>(sensorData.Humidity);
                res.Label = "Humidity";
                break;
            default:
                y = static_cast<double>(sensorData.CO2PPM);
                res.Label = "Carbon Dioxide";
                break;
            }

            res.Points.append(QPointF(t, y));
            res.Min = std::min(res.Min, y);
            res.Max = std::max(res.Max, y);
        }

        return res;
    }
}
