#include "BinaryDeserializer.h"

#ifndef BRYTEC_EMBEDDED
#include <fstream>
#endif

BinaryDeserializer::BinaryDeserializer(uint8_t* data)
    : m_data(data)
{
}

#ifndef BRYTEC_EMBEDDED
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
#endif

BinaryDeserializer ::~BinaryDeserializer()
{
    if (m_ownData)
        delete[] m_data;
}