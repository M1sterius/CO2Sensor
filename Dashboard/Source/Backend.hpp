#ifndef BACKEND_H
#define BACKEND_H

#include "Server/Server.hpp"
#include "Server/DataProcessing/DataSaver.hpp"
#include "Utilities/RunningAvgHelper.hpp"

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

        Q_INVOKABLE void onUpdateButtonClicked();
        Q_INVOKABLE void onConfigureSensorButtonClicked();
        Q_INVOKABLE void onReadingSelectionRadioButtonClicked(const int buttonId);
        Q_INVOKABLE void onSelectedDateChanged(const QDate date);
    signals:
        void updateCurrentData(int co2, double temp, double hum, QString time, QString description);
        void updateGraph(QVariantList points, double yMin, double yMax, double timestep, QString yLabel);
        void sensorStatusChanged(QString status, QString color);
    private:
        std::shared_ptr<Server> m_Server;
        std::unique_ptr<DataSaver> m_DataSaver;
        std::thread m_ServerThread;
        RunningAvgHelper<uint32_t, 10> m_Co2Avg;
        RunningAvgHelper<float, 10> m_TempAvg;
        RunningAvgHelper<float, 10> m_HumAvg;

        void OnNewMessageReceived(const std::string& message);
    };
}

#endif // BACKEND_H
