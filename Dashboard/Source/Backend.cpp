#include "Backend.hpp"
#include "Utilities/Utilities.hpp"
#include "Utilities/RangesHelper.hpp"
#include "Server/DataProcessing/Parser.hpp"

#include <QDebug>
#include <QPointF>

#include <fmt/format.h>

namespace CO2::PC
{
    Backend::Backend(QObject* parent)
    : QObject{parent}, m_GraphData(std::make_unique<GraphDataProcessor>()),
        m_SelectedDate(QDate::currentDate().toString("yyyy-MM-dd").toStdString())
    {
        // Both callbacks run on m_ServerThread, not on the main thread!!
        const std::function readCallback = [this](const std::string& msg) {
            this->OnNewMessageReceived(msg);
        };

        const std::function connectCallback = [this](const Server::ConnectEventType type) {
            const auto status = type == Server::ConnectEventType::Connected ? "Connected" : "Not connected";
            const auto color = type == Server::ConnectEventType::Connected ? "green" : "red";

            emit sensorStatusChanged(status, color);
        };

        m_Server = Server::Make(readCallback, connectCallback);
    }

    Backend::~Backend()
    {
        m_Server->Stop();
        if (m_ServerThread.joinable())
            m_ServerThread.join();
    }

    void Backend::Start()
    {
        UpdateGraphProperties();
        m_GraphData->LoadDateReadings(QDate::currentDate().toString("yyyy-MM-dd").toStdString());
        UpdateGraphPoints();

        m_ServerThread = std::thread([this]{ this->m_Server->Run(); });
    }

    void Backend::OnNewMessageReceived(const std::string& message)
    {
        const auto [error, sensorData, tag] = Parser::Parse(message);

        if (error)
            return;

        if (tag == NEW_SENSOR_READING_TAG)
        {
            m_Co2Avg.Push(sensorData.CO2PPM);
            m_TempAvg.Push(sensorData.Temperature);
            m_HumAvg.Push(sensorData.Humidity);

            const auto time = Utilities::TimestampToLastReadingTime(sensorData.Timestamp);
            const auto description = RangesHelper::GetRangeDescription(m_Co2Avg.GetAvg());

            emit updateCurrentData(m_Co2Avg.GetAvg(), m_TempAvg.GetAvg(), m_HumAvg.GetAvg(),
                QString::fromStdString(time), QString::fromStdString(description));
        }

        m_GraphData->SaveReading(sensorData);
    }

    void Backend::UpdateGraphProperties()
    {
        const auto availableDates = m_GraphData->QueryAvailableDates();
        const auto totalSize = m_GraphData->QueryTotalSizeOfFiles();

        emit setGraphProperties(availableDates, availableDates.count(),
            QString::number(totalSize / 1024) + " Kb");
    }

    void Backend::UpdateGraphPoints()
    {
        const double xMax = 24.0;
        const auto xMin = xMax - double(m_SelectedPeriodHours);

        const auto [yMin, yMax, yLabel, Points] = m_GraphData->GetGraphPoints(m_SelectedReading);
        emit updateGraph(Points, xMin, xMax, yMin, yMax, QString::fromStdString(yLabel));
    }

    void Backend::onUpdateGraphButtonClicked()
    {
        UpdateGraphProperties();
        UpdateGraphPoints();
    }

    void Backend::onConfigureSensorButtonClicked()
    {
        // TODO: Implement
        qDebug() << "Configure button clicked!";
    }

    void Backend::onReadingSelectionRadioButtonClicked(int buttonId)
    {
        m_SelectedReading = static_cast<ReadingType>(buttonId);
        UpdateGraphPoints();
    }

    void Backend::onPeriodSelectionRadioButtonClicked(const int buttonId)
    {
        m_SelectedPeriodHours = buttonId; // buttonId value directly correponds to hours
        UpdateGraphPoints();
    }

    void Backend::onSelectedDateChanged(const QDate date)
    {
        m_SelectedDate = date.toString("yyyy-MM-dd").toStdString();

        if (!m_GraphData->LoadDateReadings(m_SelectedDate))
            return; // TODO: Error popup
        UpdateGraphPoints();
    }
}
