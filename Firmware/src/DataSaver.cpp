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
        constexpr uint32_t MAX_PER_BATCH = 50;
        static uint32_t currentBatch = 0;
        static File bufferFile{};
        static File dataFile{};

        if (currentBatch >= MAX_PER_BATCH)
        {
            currentBatch = 0;
            bufferFile.close();
            bufferFile = {};
            dataFile.close();
            dataFile = {};
            return false;
        }

        if (!bufferFile)
            bufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "r+");
        if (!dataFile)
            dataFile = LittleFS.open(SENSOR_READINGS_FILE, "r");

        if (!bufferFile)
        {
            DEBUG_LOG("Failed to open buffer file to read circular buffer state!");
            return false;
        }

        if (!dataFile)
        {
            DEBUG_LOG("Failed to open data file to read sensor readings!");
            return false;
        }

        BufferState bufferState{};
        bufferFile.seek(0, SeekSet);
        bufferFile.readBytes(reinterpret_cast<char*>(&bufferState), sizeof(bufferState));

        if (bufferState.Count == 0)
            return false;

        const auto pos = bufferState.Tail * sizeof(SensorData);

        bufferState.Tail = (bufferState.Tail + 1) % CIRCULAR_BUFFER_SIZE;
        bufferState.Count--;
        bufferFile.seek(0, SeekSet);
        bufferFile.write(reinterpret_cast<uint8_t*>(&bufferState), sizeof(BufferState));

        dataFile.seek(pos, SeekSet);
        dataFile.readBytes(reinterpret_cast<char*>(&sensorData), sizeof(SensorData));

        currentBatch++;
        return true;
    }

    void DataSaver::Write(const SensorData& data)
    {
        auto readingsFile = LittleFS.open(SENSOR_READINGS_FILE, "r+");
        if (!readingsFile)
        {
            DEBUG_LOG("Failed to open sensor readings file for sensor reading write!");
            return;
        }

        readingsFile.seek(GetWritePos(), SeekSet);
        readingsFile.write(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
        readingsFile.close();

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
        auto bufferFile = LittleFS.open(SENSOR_BUFFER_FILE, "r+");
        if (!bufferFile)
        {
            DEBUG_LOG("Failed to open sensor buffer file for updating circular buffer");
            return;
        }

        BufferState bufferState{};
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
