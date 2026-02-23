#include "Server.hpp"

#include "fmt/format.h"

#include <iostream>

Server::Server()
{
    m_Buffer = std::vector<char>(1024);
    m_Worker = std::thread([this]{ m_Context.run(); });

    m_Acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_Context,
        asio::ip::tcp::endpoint(asio::ip::tcp::v4(), SERVER_PORT));
}

Server::~Server()
{
    m_Context.stop();
    if (m_Worker.joinable())
        m_Worker.join();
}

void Server::Start()
{
    WaitForClientConnection();
}

std::shared_ptr<Server> Server::Make()
{
    auto ptr = std::make_shared<Server>();
    ptr->Start();
    return ptr;
}

void Server::WaitForClientConnection()
{
    auto self = shared_from_this();
    m_Acceptor->async_accept([self](const asio::error_code& ec, asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            fmt::println("[SERVER] Connected client at {}:{}.", socket.remote_endpoint().address().to_string(),
                socket.remote_endpoint().port());

            self->m_DeviceSocket = std::make_shared<asio::ip::tcp::socket>(std::move(socket));
            self->ReceiveData();

            // No new call to WaitForClientConnection because only one device is currently supported
        }
        else
        {
            fmt::println("[SERVER] Failed to connect client at {}:{}.", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());
        }
    });
}

void Server::ReceiveData()
{
    auto self = shared_from_this();
    m_DeviceSocket->async_read_some(asio::buffer(m_Buffer), [self](const asio::error_code& ec, const size_t size)
    {
        if (!ec)
        {
            std::cout.write(self->m_Buffer.data(), self->m_Buffer.size());
            self->ReceiveData();
        }
        else
        {

        }
    });
}
