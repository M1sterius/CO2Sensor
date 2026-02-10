#define ASIO_STANDALONE
#include "asio.hpp"
#include "fmt/format.h"

#include <iostream>
#include <cstdint>
#include <thread>
#include <string>
#include <memory>

constexpr uint16_t SERVER_PORT = 16546;

std::shared_ptr<asio::ip::tcp::socket> g_Socket;
char g_Buffer[1024];

void ReadSocket(std::shared_ptr<asio::ip::tcp::socket> socket)
{
    socket->async_read_some(asio::buffer(g_Buffer, sizeof(g_Buffer)), [socket](const asio::error_code& ec, const size_t size)
    {
        std::cout.write(g_Buffer, static_cast<std::streamsize>(size));
        ReadSocket(socket);
    });
}

void WaitForClientConnection(asio::ip::tcp::acceptor& acceptor)
{
    acceptor.async_accept([](const asio::error_code& ec, asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            fmt::println("[SERVER] Connected client at {}:{}.", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());

            g_Socket = std::make_shared<asio::ip::tcp::socket>(std::move(socket));
            ReadSocket(g_Socket);

            // No new call to WaitForClientConnection because only one device is currently supported
        }
        else
        {
            fmt::println("[SERVER] Failed to connect client at {}:{}.", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());
        }
    });
}

int main()
{
    asio::io_context context;
    auto worker = std::thread([&context]{ context.run(); });

    auto acceptor = asio::ip::tcp::acceptor(context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), SERVER_PORT));
    WaitForClientConnection(acceptor);

    std::string line;
    while (true)
    {
        std::getline(std::cin, line);

        if (line == "exit")
            break;
    }

    context.stop();
    if (worker.joinable())
        worker.join();
}