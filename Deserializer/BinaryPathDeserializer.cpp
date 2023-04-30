#include "BinaryPathDeserializer.h"

#if __has_include(<filesystem>)

#include <cstring>
#include <fstream>

namespace Brytec {

BinaryPathDeserializer::BinaryPathDeserializer(std::filesystem::path path)
{
    std::ifstream is(path, std::ifstream::binary);
    if (is) {
        is.seekg(0, is.end);
        m_dataLength = is.tellg();
        is.seekg(0, is.beg);

        m_data = new uint8_t[m_dataLength];
        is.read((char*)m_data, m_dataLength);

        is.close();
    }
}

BinaryPathDeserializer ::~BinaryPathDeserializer()
{
    delete[] m_data;
}

bool BinaryPathDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
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
#endif