#include "Server.hpp"
#include "Protocol.hpp"

#include "fmt/format.h"

namespace CO2::PC
{
    Server::Server(const std::function<void(const std::string&)>& readCallback)
        : m_WorkGuard(asio::make_work_guard(m_Context)), m_TimeoutTimer(m_Context),
          m_ReadCallback(readCallback)
    {
        m_Buffer = std::vector<char>(1024);
        m_Worker = std::thread([this]{ m_Context.run(); });

        m_Acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_Context,
            asio::ip::tcp::endpoint(asio::ip::tcp::v4(), SERVER_PORT));
    }

    Server::~Server()
    {
        Disconnect();

        m_Context.stop();
        if (m_Worker.joinable())
            m_Worker.join();
    }

    void Server::Start()
    {
        WaitForClientConnection();
        fmt::println("[SERVER] Server started.");
    }

    void Server::Run()
    {
        while (true)
        {
            auto message = m_MessageQueue.wait_and_pop();
            m_ReadCallback(message);
        }
    }

    std::shared_ptr<Server> Server::Make(const std::function<void(const std::string&)>& readCallback)
    {
        auto ptr = std::make_shared<Server>(readCallback);
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

                self->m_Socket = std::make_unique<asio::ip::tcp::socket>(std::move(socket));
                self->ReceiveData();

                self->m_LastPacket = std::chrono::steady_clock::now();
                self->StartTimeoutCheck();
            }
            else
            {
                fmt::println("[SERVER] Failed to connect client at {}:{}.", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());
            }
        });
    }

    void Server::StartTimeoutCheck()
    {
        constexpr auto TIMEOUT_TIME = static_cast<uint32_t>(SENSOR_READ_DELAY * 1.5);

        m_TimeoutTimer.expires_after(std::chrono::milliseconds(TIMEOUT_TIME));

        auto self = shared_from_this();
        m_TimeoutTimer.async_wait([TIMEOUT_TIME, self](const asio::error_code& ec)
        {
            if (ec)
                return;

            if (const auto now = std::chrono::steady_clock::now();
                now - self->m_LastPacket > std::chrono::milliseconds(TIMEOUT_TIME))
            {
                fmt::println("[SERVER] Client timed out!");
                self->Disconnect();
                return;
            }

            self->StartTimeoutCheck();
        });
    }

    void Server::ReceiveData()
    {
        auto self = shared_from_this();
        m_Socket->async_read_some(asio::buffer(m_Buffer),
            [self](const asio::error_code& ec, const size_t size)
        {
            if (!ec)
            {
                self->m_StringBuffer.append(self->m_Buffer.data(), size);

                size_t pos = 0;
                while ((pos = self->m_StringBuffer.find('\n')) != std::string::npos)
                {
                    auto message = self->m_StringBuffer.substr(0, pos);
                    self->m_StringBuffer.erase(0, pos + 1);

                    // handle CRLF line endings coming from esp32
                    if (!message.empty() && message.back() == '\r')
                        message.pop_back();

                    self->m_MessageQueue.push(message);
                }

                self->m_LastPacket = std::chrono::steady_clock::now();
                self->ReceiveData();
            }
            else
            {
                fmt::println("[SERVER] Client disconnected! {}.", ec.message());
                self->Disconnect();
            }
        });
    }

    void Server::Disconnect()
    {
        if (!m_Socket)
            return;

        asio::error_code ec;

        ec = m_Socket->cancel(ec);
        ec = m_Socket->close(ec);
        m_TimeoutTimer.cancel();
        m_Socket.reset();

        WaitForClientConnection();
    }
}
