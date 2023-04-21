#include "BinaryDeserializer.h"

#include "EBrytecConfig.h"
#include <string.h>
#ifndef BRYTEC_EMBEDDED
#include <fstream>
#endif

namespace Brytec {

BinaryDeserializer::BinaryDeserializer(const uint8_t* data, uint32_t length)
    : m_data(data)
    , m_dataLength(length)
{
}

#ifndef BRYTEC_EMBEDDED
BinaryDeserializer::BinaryDeserializer(std::filesystem::path path)
{
    std::ifstream is(path, std::ifstream::binary);
    if (is) {
        is.seekg(0, is.end);
        m_dataLength = is.tellg();
        is.seekg(0, is.beg);

        m_data = new uint8_t[m_dataLength];
        is.read((char*)m_data, m_dataLength);

        is.close();

        m_ownData = true;
    }
}
#endif

BinaryDeserializer::~BinaryDeserializer()
{
    if (m_ownData)
        delete[] m_data;
}

bool BinaryDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
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