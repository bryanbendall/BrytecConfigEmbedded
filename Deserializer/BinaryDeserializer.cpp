#include "BinaryDeserializer.h"

#include "EBrytecConfig.h"
#include <cstring>
#include <fstream>

BinaryDeserializer::~BinaryDeserializer()
{
    if (m_ownData)
        delete[] m_data;
}

bool BinaryDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    if (data) {
        memcpy(data, &m_data[m_currentOffset], dataSize);
        m_currentOffset += dataSize;
        return true;
    } else {
        return false;
    }
}

void BinaryDeserializer::setData(uint8_t* data, uint32_t length)
{
    m_data = data;
}

void BinaryDeserializer::setDataFromPath(std::filesystem::path path)
{
    std::ifstream is(path, std::ifstream::binary);
    if (is) {
        is.seekg(0, is.end);
        int length = is.tellg();
        is.seekg(0, is.beg);

        m_data = new uint8_t[length];
        is.read((char*)m_data, length);

        is.close();

        m_ownData = true;
    }
}