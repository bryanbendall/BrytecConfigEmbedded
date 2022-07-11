#include "BinaryDeserializer.h"

#include <fstream>

BinaryDeserializer::BinaryDeserializer(uint8_t* data)
    : m_data(data)
{
}

BinaryDeserializer::BinaryDeserializer(std::filesystem::path path)
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

BinaryDeserializer ::~BinaryDeserializer()
{
    if (m_ownData)
        delete[] m_data;
}