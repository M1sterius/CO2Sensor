#pragma once

#include <list>
#include <cstdint>

namespace CO2::PC
{
    template<typename T, size_t maxValues>
    class RunningAvgHelper
    {
    public:
        RunningAvgHelper() = default;
        ~RunningAvgHelper() = default;

        void Push(const T& value)
        {
            if (m_Values.size() + 1 > maxValues)
            {
                const auto frontVal = m_Values.front();
                m_Values.pop_front();

                m_Sum -= frontVal;
                m_Sum += value;
                m_Values.push_back(value);
            }
            else
            {
                m_Values.push_back(value);
                m_Sum += value;
            }
        }

        T GetAvg() const
        {
            return m_Sum / static_cast<T>(m_Values.size());
        }
    private:
        T m_Sum{0};
        std::list<T> m_Values{};
    };
}
