#pragma once

#include <stdint.h>
#include <string.h>

namespace Brytec {

class BinaryBufferSerializer {

public:
    BinaryBufferSerializer(uint8_t* buffer, uint32_t bufferLength);
    uint32_t getWroteLenth() { return m_currentOffset; }

    template <typename T>
    bool writeRaw(T data)
    {
        uint32_t size = sizeof(T);

        // Wont fit in the buffer
        if (m_currentOffset + size > m_bufferLength)
            return false;

        memcpy(&m_buffer[m_currentOffset], &data, size);
        m_currentOffset += size;

        return true;
    }

private:
    uint8_t* m_buffer;
    uint32_t m_bufferLength;
    uint32_t m_currentOffset = 0;
};

}