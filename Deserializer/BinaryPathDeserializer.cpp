#include "BinaryPathDeserializer.h"

#if __has_include(<filesystem>)

#include <cstring>
#include <fstream>
#include <iostream>

namespace Brytec {

BinaryPathDeserializer::BinaryPathDeserializer(std::filesystem::path path)
{
    std::ifstream is(path, std::ifstream::binary);
    if (is) {

        // Clear whitespace removal flag
        is.unsetf(std::ios::skipws);

        is.seekg(0, std::ios::end);
        m_dataLength = is.tellg();
        is.seekg(0, std::ios::beg);

        m_vec.reserve(m_dataLength);

        // Read the data
        m_vec.insert(m_vec.begin(),
            std::istream_iterator<uint8_t>(is),
            std::istream_iterator<uint8_t>());

        if (m_vec.size() != m_dataLength) {
            std::cout << "BinaryPathDeserializer did not real all of the file!" << std::endl;
            m_dataLength = 0;
            m_vec.clear();
        }

        is.close();
    }
}

BinaryPathDeserializer ::~BinaryPathDeserializer()
{
}

bool BinaryPathDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    if (m_currentOffset + dataSize > m_dataLength)
        return false;

    if (data) {
        memcpy(data, &m_vec[m_currentOffset], dataSize);
        m_currentOffset += dataSize;
        return true;
    } else {
        return false;
    }
}

void BinaryPathDeserializer::getRawData(uint8_t* dest, uint32_t offset, uint32_t length)
{
    if (offset + length > m_dataLength)
        return;

    memcpy(dest, &m_vec[offset], length);
}
}
#endif