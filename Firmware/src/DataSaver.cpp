#include "DataSaver.hpp"
#include "Debug.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    DataSaver::DataSaver() = default;
    DataSaver::~DataSaver() = default;
    
    bool DataSaver::Begin()
    {
        if (!LittleFS.begin(true))
        {
            DEBUG_LOG("Failed to initialize LittleFS.");
            return false;
        }

        Serial.printf("FS total: %d\n", LittleFS.totalBytes());
        Serial.printf("FS used: %d\n", LittleFS.usedBytes());

        m_File = LittleFS.open("/data.txt", "rw");
        if (!m_File)
        {
            DEBUG_LOG("Failed to open data file.");
            return false;
        }

        return true;
    }

    bool DataSaver::Read(char* buffer, const uint32_t bufferSize)
    {
        return false;
    }

    void DataSaver::Write(const SensorData& data)
    {

    }

    const char* DataSaver::SensorDataToString(const SensorData& data)
    {
        static char buffer[100];
        snprintf(buffer, sizeof(buffer), "%u,%u,%u,%u", data.Timestamp, data.Humidity, data.CO2PPM);
        return buffer;
    }
}