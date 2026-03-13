#define ASIO_STANDALONE
#include "asio.hpp"
#include "fmt/format.h"

#include "Server.hpp"
#include "Protocol.hpp"

#include <iostream>
#include <string>

int32_t main(int32_t argc, char** argv)
{
    const auto server = CO2::PC::Server::Make([](const std::string& msg)
    {
        std::cout << msg << '\n';
    });

    server->Run();
}
