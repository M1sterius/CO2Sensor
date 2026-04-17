#pragma once

#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

namespace CO2
{
    template<typename T>
    class CVQueue
    {
    public:
        CVQueue() = default;
        ~CVQueue() = default;
        CVQueue(const CVQueue&) = delete;
        CVQueue& operator = (const CVQueue&) = delete;

        const T& front()
        {
            std::unique_lock lock(m_Mutex);
            return m_Queue.front();
        }

        const T& back()
        {
            std::unique_lock lock(m_Mutex);
            return m_Queue.back();
        }

        void push(const T& value)
        {
            {
                std::unique_lock lock(m_Mutex);
                m_Queue.push(value);
            }

            m_CV.notify_one();
        }

        std::optional<T> wait_and_pop()
        {
            std::unique_lock lock(m_Mutex);
            m_CV.wait(lock, [this]{ return !m_Queue.empty() || m_Done; });

            if (m_Queue.empty() && m_Done) // TODO: should it really be &&?
                return std::nullopt;

            auto temp = std::move(m_Queue.front());
            m_Queue.pop();
            return temp;
        }

        void stop()
        {
            {
                std::unique_lock lock(m_Mutex);
                m_Done = true;
            }

            m_CV.notify_all();
        }

        size_t size()
        {
            std::unique_lock lock(m_Mutex);
            return m_Queue.size();
        }

        bool empty()
        {
            std::unique_lock lock(m_Mutex);
            return m_Queue.empty();
        }
    private:
        bool m_Done{false};
        std::mutex m_Mutex;
        std::queue<T> m_Queue;
        std::condition_variable m_CV;
    };
}
