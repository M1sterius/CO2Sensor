#include "Server/Server.hpp"
#include "Server/Data/DataParser.hpp"
#include "Server/Data/DataSaver.hpp"
#include "Protocol.hpp"
#include "DataHandler.hpp"

#include <fmt/format.h>

#include <string>
#include <iostream>

int32_t main(int32_t argc, char** argv)
{
    auto dataHandler = CO2::PC::DataHandler();

    const std::function callback = [&dataHandler](const std::string& message)
    {
        dataHandler.Parse(message);
    };

    const auto server = CO2::PC::Server::Make(callback);
    server->Run();
}
