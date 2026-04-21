#pragma once

#include "Protocol.hpp"
#include "ReadingType.hpp"

#include <QPointF>
#include <QVariantList>

#include <string>
#include <fstream>
#include <set>
#include <mutex>

namespace CO2::PC
{
    class GraphDataProcessor
    {
    private:
        struct SensorDataComparator
        {
            bool operator () (const SensorData& a, const SensorData& b) const {
                return a.Timestamp < b.Timestamp;
            }
        };

        struct ReadingsSaveState
        {
            std::string Date{};
            std::fstream File{};
            uint32_t Count{0};
        };
    public:
        struct GraphPointsContainer
        {
            double Min{0.0};
            double Max{0.0};
            std::string Label{"Label"};
            QVariantList Points{};
        };

        GraphDataProcessor();
        ~GraphDataProcessor();

        bool LoadDateReadings(const std::string& date);
        GraphPointsContainer GetGraphPoints(const ReadingType readingType);

        void SaveReading(const SensorData& sensorData);

        QVariantList QueryAvailableDates() const;
        size_t QueryTotalSizeOfFiles() const;
    private:
        std::multiset<SensorData, SensorDataComparator> m_Storage;
        ReadingsSaveState m_SaveState;
        std::mutex m_Mutex;
    };
}
