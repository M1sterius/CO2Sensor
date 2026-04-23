#include "SerialHandler.hpp"
#include "Protocol.hpp"

#include <fmt/format.h>

#include <string>

namespace CO2::PC
{
    bool SerialHandler::IsSensorConnected()
    {
        return !FindPort().empty();
    }

    bool SerialHandler::SendConfig(const std::string& wifiSsid, const std::string& wifiPassword,
        const std::string& serverIp)
    {
        const auto port = FindPort();
        if (port.empty())
            return false;

        try
        {
            serial::Serial esp_serial(port, SERIAL_BAUD, serial::Timeout::simpleTimeout(1000));

            if (esp_serial.isOpen())
            {
                const auto message = fmt::format("{};{};{}", wifiSsid, wifiPassword, serverIp);
                esp_serial.write(message);

                // TODO: Response check would be nice

                esp_serial.close();
            }
        }
        catch (std::exception &e)
        {
            fmt::println("Failed to send configuration to the sensor!");
            return false;
        }

        return true;
    }

    std::string SerialHandler::FindPort()
    {
        for (const auto& port : serial::list_ports())
        {
            const auto hw_id = port.hardware_id;

            if (hw_id.find("10C4") != std::string::npos || // CP210x
                hw_id.find("1A86") != std::string::npos || // CH340
                hw_id.find("303A") != std::string::npos || // Espressif native
                hw_id.find("0403") != std::string::npos)   // FTDI
            {

                return port.port;
            }
        }

        return "";
    }
}
