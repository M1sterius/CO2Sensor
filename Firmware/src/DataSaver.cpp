#include "DataSaver.hpp"
#include "Utilities/Debug.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    bool DataSaver::BatchState::OpenFiles()
    {
        if (BufferFile && ReadingsFile)
            return true;

        BufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "r+");
        ReadingsFile = LittleFS.open(SENSOR_READINGS_FILE, "r");

        if (!BufferFile || !ReadingsFile)
        {
            DEBUG_LOG("Failed to open files in BatchState!");
            CloseFiles();
            return false;
        }

        return true;
    }

    void DataSaver::BatchState::CloseFiles()
    {
        if (BufferFile) BufferFile.close();
        if (ReadingsFile) ReadingsFile.close();
    }

    DataSaver::DataSaver() = default;
    DataSaver::~DataSaver() = default;
    
    bool DataSaver::Begin()
    {
        if (!LittleFS.begin(true))
        {
            DEBUG_LOG("Failed to initialize LittleFS.");
            return false;
        }

        if (!LittleFS.exists("/sensor"))
            LittleFS.mkdir("/sensor");

        if (!LittleFS.exists(SENSOR_BUFFER_FILE))
        {
            auto bufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "w");
            if (!bufferFile)
            {
                DEBUG_LOG("Failed to create sensor circular buffer file!");
                return false;
            }
            else
                DEBUG_LOG("Sensor circular buffer file successfully created!");

            BufferState buffer{};
            bufferFile.write(reinterpret_cast<uint8_t*>(&buffer), sizeof(buffer));

            bufferFile.close();
        }

        if (!LittleFS.exists(SENSOR_READINGS_FILE))
        {
            auto readingsFile = LittleFS.open(SENSOR_READINGS_FILE, "w");
            if (!readingsFile)
            {
                DEBUG_LOG("Failed to create sensor readings data file!");
                return false;
            }
            else
                DEBUG_LOG("Sensor data file successfully created!");

            readingsFile.close();
        }

        return true;
    }

    bool DataSaver::Read(SensorData& sensorData)
    {
        if (m_CurrentBatch.Count > MAX_READINGS_PER_BATCH)
        {
            m_CurrentBatch.CloseFiles();
            m_CurrentBatch = BatchState();
            return false;
        }

        if (!m_CurrentBatch.OpenFiles())
            return false;

        // Read buffer state
        BufferState bufferState{};
        m_CurrentBatch.BufferFile.seek(0, SeekSet);
        m_CurrentBatch.BufferFile.readBytes(reinterpret_cast<char*>(&bufferState), sizeof(BufferState));

        if (bufferState.Count == 0)
        {
            m_CurrentBatch.CloseFiles();
            return false;
        }

        m_CurrentBatch.ReadingsFile.seek(bufferState.Tail * sizeof(SensorData), SeekSet);
        m_CurrentBatch.ReadingsFile.readBytes(reinterpret_cast<char*>(&sensorData), sizeof(SensorData));

        bufferState.Tail = (bufferState.Tail + 1) % CIRCULAR_BUFFER_SIZE;
        bufferState.Count--;
        m_CurrentBatch.Count++;

        // Write buffer state
        m_CurrentBatch.BufferFile.seek(0, SeekSet);
        m_CurrentBatch.BufferFile.write(reinterpret_cast<uint8_t*>(&bufferState), sizeof(BufferState));
        m_CurrentBatch.BufferFile.flush();

        return true;
    }

    void DataSaver::Write(const SensorData& data)
    {
        auto bufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "r+");
        auto readingsFile = LittleFS.open(SENSOR_READINGS_FILE, "r+");

        if (!bufferFile || !readingsFile)
        {
            DEBUG_LOG("Failed to open files to save a sensor reading!");

            if (bufferFile) bufferFile.close();
            if (readingsFile) readingsFile.close();

            return;
        }

        // Read buffer state
        BufferState bufferState{};
        bufferFile.seek(0, SeekSet);
        bufferFile.readBytes(reinterpret_cast<char*>(&bufferState), sizeof(BufferState));

        // Write sensor reading to the file
        const auto readingWritePos = bufferState.Head * sizeof(SensorData);
        readingsFile.seek(readingWritePos, SeekSet);
        readingsFile.write(reinterpret_cast<const uint8_t*>(&data), sizeof(SensorData));

        // Update buffer state
        if (bufferState.Count < CIRCULAR_BUFFER_SIZE)
            bufferState.Count++;
        else
            bufferState.Tail = (bufferState.Tail + 1) % CIRCULAR_BUFFER_SIZE;

        bufferState.Head = (bufferState.Head + 1) % CIRCULAR_BUFFER_SIZE;

        // Write updated buffer state to the file
        bufferFile.seek(0, SeekSet);
        bufferFile.write(reinterpret_cast<uint8_t*>(&bufferState), sizeof(bufferState));

        bufferFile.close();
        readingsFile.close();
    }
}
