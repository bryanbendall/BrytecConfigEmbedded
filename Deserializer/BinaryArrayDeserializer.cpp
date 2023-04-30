#include "BinaryArrayDeserializer.h"

#include <string.h>

namespace Brytec {

BinaryArrayDeserializer::BinaryArrayDeserializer(const uint8_t* data, uint32_t length)
    : m_data(data)
    , m_dataLength(length)
{
}

bool BinaryArrayDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    if (m_currentOffset + dataSize > m_dataLength)
        return false;

    if (data) {
        memcpy(data, &m_data[m_currentOffset], dataSize);
        m_currentOffset += dataSize;
        return true;
    } else {
        return false;
    }
}
}