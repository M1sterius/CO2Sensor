#include "DataSaver.hpp"
#include "Utilities/Debug.hpp"
#include "../../Protocol/Protocol.hpp"

namespace CO2::Firmware
{
    struct BufferState
    {
        uint32_t Head{0};
        uint32_t Tail{0};
        uint32_t Count{0};
    };

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
            auto dataFile = LittleFS.open(SENSOR_READINGS_FILE, "w");
            if (!dataFile)
            {
                DEBUG_LOG("Failed to create sensor readings data file!");
                return false;
            }
            else
                DEBUG_LOG("Sensor data file successfully created!");

            dataFile.close();
        }

        return true;
    }

    bool DataSaver::Read(char* buffer, const uint32_t bufferSize)
    {
        return false;
    }

    void DataSaver::Write(const SensorData& data)
    {
        auto dataFile = LittleFS.open(SENSOR_READINGS_FILE, "r+");
        if (!dataFile)
        {
            DEBUG_LOG("Failed to open sensor data file for sensor reading write!");
            return;
        }

        dataFile.seek(GetWritePos(), SeekSet);
        dataFile.write(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
        dataFile.close();

        UpdateCircularBuffer();
    }

    uint32_t DataSaver::GetWritePos()
    {
        auto bufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "r");
        if (!bufferFile)
        {
            DEBUG_LOG("Failed to open sensor buffer file for reading circular buffer head!");
            return 0;
        }

        uint32_t head = 0;

        bufferFile.seek(0, SeekSet);
        bufferFile.readBytes(reinterpret_cast<char*>(&head), sizeof(head));
        bufferFile.close();

        return head * sizeof(SensorData);
    }

    void DataSaver::UpdateCircularBuffer()
    {
        BufferState bufferState{};

        auto bufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "r+");
        if (!bufferFile)
        {
            DEBUG_LOG("Failed to open sensor buffer file for updating circular buffer");
            return;
        }

        bufferFile.readBytes(reinterpret_cast<char*>(&bufferState), sizeof(bufferState));

        if (bufferState.Count < CIRCULAR_BUFFER_SIZE)
            bufferState.Count++;
        else
            bufferState.Tail = (bufferState.Tail + 1) % CIRCULAR_BUFFER_SIZE;

        bufferState.Head = (bufferState.Head + 1) % CIRCULAR_BUFFER_SIZE;

        Serial.printf("Head: %u Tail: %u Count: %u\n", bufferState.Head, bufferState.Tail, bufferState.Count);
        bufferFile.seek(0, SeekSet);
        bufferFile.write(reinterpret_cast<uint8_t*>(&bufferState), sizeof(bufferState));
        bufferFile.close();
    }
}
