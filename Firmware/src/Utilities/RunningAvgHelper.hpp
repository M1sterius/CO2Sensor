#pragma once

#include "CircularBuffer.hpp"

#include <cstdint>

namespace CO2::Firmware
{
    template<typename T, uint32_t Size>
    class RunningAvgHelper
    {
    public:
        RunningAvgHelper() = default;
        ~RunningAvgHelper() = default;

        void Push(const T& value)
        {
            if (m_Count + 1 > Size)
            {
                m_Sum -= m_Buffer.Oldest();
                m_Count--;
            }

            m_Sum += value;
            m_Buffer.Push(value);
            m_Count++;
        }

        T GetAvg()
        {
            return m_Sum / static_cast<T>(m_Count);
        }
    private:
        CircularBuffer<T, Size> m_Buffer;
        T m_Sum{0.0};
        uint32_t m_Count{0};
    };
}
