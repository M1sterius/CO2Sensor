#include "DataSaver.hpp"
#include "Utilities/Debug.hpp"
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
}