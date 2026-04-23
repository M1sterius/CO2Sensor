#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>

#include "Utilities/CVQueue.hpp"

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>
#include <chrono>
#include <variant>
#include <functional>

namespace CO2::PC
{
    constexpr uint16_t SERVER_PORT = 16546;

    class Server : public std::enable_shared_from_this<Server>
    {
    public:
        enum class ConnectEventType
        {
            Connected,
            Disconnected
        };

        static std::shared_ptr<Server> Make(const std::function<void(const std::string&)>& readCallback,
            const std::function<void(const ConnectEventType)>& connectCallback);

        Server(const std::function<void(const std::string&)>& readCallback,
            const std::function<void(const ConnectEventType)>& connectCallback);
        ~Server();

        std::string GetLocalIP();

        void Start();
        void Run();
        void Stop();
    private:
        using MessageVariant = std::variant<std::string, ConnectEventType>;

        asio::io_context m_Context;
        std::thread m_Worker;
        bool m_Running;
        std::vector<char> m_Buffer;
        std::string m_StringBuffer;
        CVQueue<MessageVariant> m_MessageQueue;
        asio::executor_work_guard<asio::io_context::executor_type> m_WorkGuard;
        std::unique_ptr<asio::ip::tcp::acceptor> m_Acceptor;
        std::unique_ptr<asio::ip::tcp::socket> m_Socket;
        asio::steady_timer m_TimeoutTimer;
        std::chrono::steady_clock::time_point m_LastPacket;
        std::function<void(const std::string&)> m_ReadCallback;
        std::function<void(const ConnectEventType)> m_ConnectCallback;

        void WaitForClientConnection();
        void StartTimeoutCheck();
        void ReceiveData();
        void Disconnect();
    };
}
