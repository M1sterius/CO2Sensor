#pragma once

#define ASIO_STANDALONE
#include "asio.hpp"

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

constexpr uint16_t SERVER_PORT = 16546;

class Server : public std::enable_shared_from_this<Server>
{
public:
    Server();
    ~Server();

    void Start();

    static std::shared_ptr<Server> Make();
private:
    asio::io_context m_Context;
    std::thread m_Worker;
    std::vector<char> m_Buffer;
    std::unique_ptr<asio::ip::tcp::acceptor> m_Acceptor;
    std::shared_ptr<asio::ip::tcp::socket> m_DeviceSocket;

    void WaitForClientConnection();
    void ReceiveData();
};
