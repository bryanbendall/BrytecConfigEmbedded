#pragma once

namespace Brytec {

template <typename T, int Size>
class RingBuffer {

public:
    void add(const T& element)
    {
        if (m_size >= Size)
            return;

        m_buffer[m_currentIndex] = element;
        m_size++;
        m_currentIndex = (m_currentIndex + 1) % Size;
    }

    T get()
    {
        if (m_size == 0)
            return T();

        int index = m_currentIndex - m_size;
        if (index < 0)
            index = Size + index;

        m_size--;
        return m_buffer[index];
    }

    uint32_t size()
    {
        return m_size;
    }

private:
    T m_buffer[Size];
    uint32_t m_currentIndex = 0;
    uint32_t m_size = 0;
};
}