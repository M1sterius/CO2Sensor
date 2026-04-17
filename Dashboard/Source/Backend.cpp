#include "Backend.hpp"
#include "Server/DataProcessing/DataParser.hpp"

#include <QDebug>
#include <QPointF>

#include <fmt/format.h>

#include <random>

namespace CO2::PC
{
    Backend::Backend(QObject *parent)
        : QObject{parent}, m_DataSaver(std::make_unique<DataSaver>())
    {
        m_Server = Server::Make([this](const std::string& msg) {
            this->OnNewMessageReceived(msg);
        });
    }

    Backend::~Backend()
    {
        m_Server->Stop();
        if (m_ServerThread.joinable())
            m_ServerThread.join();
    }

    void Backend::Start()
    {
        m_ServerThread = std::thread([this]{ this->m_Server->Run(); });
    }

    void Backend::OnNewMessageReceived(const std::string &message)
    {
        const auto [error, sensorData, tag] = DataParser::Parse(message);

        if (error)
            return;

        if (tag == NEW_SENSOR_READING_TAG)
        {
            m_Co2Avg.Push(sensorData.CO2PPM);
            m_TempAvg.Push(sensorData.Temperature);
            m_HumAvg.Push(sensorData.Humidity);

            emit updateCurrentData(m_Co2Avg.GetAvg(), m_TempAvg.GetAvg(), m_HumAvg.GetAvg(),
                "No time", "No description");
        }

        m_DataSaver->SaveReading(sensorData);
    }

    double rng(double min, double max)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> distrib(min, max);

        return distrib(gen);
    }

    void Backend::onUpdateButtonClicked()
    {
        qDebug() << "Update button clicked!";

        QVariantList points;

        for (int i = 0; i < 100; i++)
        {
            points.append(QPointF((double)i, rng(0, 10)));
        }

        emit updateGraph(points, 0.0, 10, 10 / 100, "Test label");
    }

    void Backend::onConfigureSensorButtonClicked()
    {
        qDebug() << "Configure button clicked!";

        static int val = 654;

        emit updateCurrentData(val++, 22.5, 40.2, "2026.04.16 20:05:01", "Test description");
    }

    void Backend::onReadingSelectionRadioButtonClicked(int buttonId)
    {
        qDebug() << buttonId;
    }

    void Backend::onSelectedDateChanged(const QDate date)
    {
        qDebug() << date.toString();
    }
}
