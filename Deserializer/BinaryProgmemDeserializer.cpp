#include "BinaryProgmemDeserializer.h"

#if __has_include(<avr/pgmspace.h>)

#include <avr/pgmspace.h>

namespace Brytec {

BinaryProgmemDeserializer::BinaryProgmemDeserializer(const uint8_t* data, uint32_t length)
    : m_data(data)
    , m_dataLength(length)
{
}

bool BinaryProgmemDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    if (m_currentOffset + dataSize > m_dataLength)
        return false;

    if (data) {
        memcpy_P(data, &m_data[m_currentOffset], dataSize);
        m_currentOffset += dataSize;
        return true;
    } else {
        return false;
    }
}

}

#endif