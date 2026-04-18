#pragma once

#include "Protocol.hpp"

#include <QPointF>

#include <string>
#include <fstream>
#include <set>

namespace CO2::PC
{
    class GraphDataProcessor
    {
    private:
        struct SensorDataComparator
        {
            bool operator()(const SensorData& a, const SensorData& b) const {
                return a.Timestamp < b.Timestamp;
            }
        };
    public:
        struct GraphPointsContainer
        {
            double Min{0.0};
            double Max{0.0};
            std::string Label{"Label"};
            std::vector<QPointF> Points{};
        };

        GraphDataProcessor();
        ~GraphDataProcessor();

        bool LoadDateData(const std::string& date);
        GraphPointsContainer GetGraphPoints(const uint32_t readingId);
    private:
        std::fstream m_File;
        std::multiset<SensorData, SensorDataComparator> m_Storage;
    };
}
