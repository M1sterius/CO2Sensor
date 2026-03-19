#pragma once

#include <cstdint>

namespace CO2::Firmware
{
    template<typename T, uint32_t Size>
    class CircularBuffer
    {
    public:
        CircularBuffer() = default;
        ~CircularBuffer() = default;

        void Push(const T& value)
        {
            m_Buffer[m_Head] = value;

            if (m_Count < Size)
                m_Count++;
            else
                m_Tail = (m_Tail + 1) % Size;

            m_Head = (m_Head + 1) % Size;
        }

        const T& Oldest() const { return m_Buffer[m_Tail]; }
        const T& Newest() const { return m_Buffer[m_Head]; }
    private:
        T m_Buffer[Size];
        uint32_t m_Head{0};
        uint32_t m_Tail{0};
        uint32_t m_Count{0};
    };
}
