#pragma once

#include "Server/Server.hpp"
#include "Server/DataProcessing/GraphDataProcessor.hpp"
#include "Utilities/RunningAvgHelper.hpp"
#include "ReadingType.hpp"

#include <QObject>
#include <QString>
#include <QDate>
#include <QVariantList>

#include <thread>

namespace CO2::PC
{
    class Backend : public QObject
    {
        Q_OBJECT
    public:
        explicit Backend(QObject* parent = nullptr);
        ~Backend();

        void Start();

        Q_INVOKABLE void onUpdateGraphButtonClicked();
        Q_INVOKABLE void onConfigureSensorButtonClicked();
        Q_INVOKABLE void onSubmitSensorConfigButtonClicked(const QString ssid, const QString password, const QString ip);
        Q_INVOKABLE void onReadingSelectionRadioButtonClicked(const int buttonId);
        Q_INVOKABLE void onPeriodSelectionRadioButtonClicked(const int buttonId);
        Q_INVOKABLE void onSelectedDateChanged(const QDate date);
    signals:
        void updateCurrentData(int co2, double temp, double hum, QString time, QString description);
        void updateGraph(QVariantList points, double xMin, double xMax, double yMin, double yMax, QString yLabel);
        void sensorStatusChanged(QString status, QString color);
        void setGraphProperties(QVariantList availableDates, int availableDatesCount, QString totalSize);
        void sensorReadyStatusChanged(bool ready);
    private:
        std::shared_ptr<Server> m_Server;
        std::thread m_ServerThread;
        std::unique_ptr<GraphDataProcessor> m_GraphData;
        RunningAvgHelper<uint32_t, 10> m_Co2Avg;
        RunningAvgHelper<float, 10> m_TempAvg;
        RunningAvgHelper<float, 10> m_HumAvg;
        std::string m_SelectedDate{};
        ReadingType m_SelectedReading{ReadingType::CarbonDioxide};
        uint32_t m_SelectedPeriodHours{1};

        void UpdateGraphProperties();
        void UpdateGraphPoints();

        void OnNewMessageReceived(const std::string& message);
    };
}

