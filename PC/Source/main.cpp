#define ASIO_STANDALONE
#include "asio.hpp"
#include "fmt/format.h"

#include "Server.hpp"
#include "Protocol.hpp"
#include "DataHandler.hpp"

#include <string>
#include <iostream>

int32_t main(int32_t argc, char** argv)
{
    auto dataHandler = CO2::PC::DataHandler();

    const std::function callback = [&dataHandler](const std::string& message){
        dataHandler.Parse(message);
    };

    const auto server = CO2::PC::Server::Make(callback);
    server->Run();
}
