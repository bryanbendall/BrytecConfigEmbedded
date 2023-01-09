#pragma once

#include "EBrytecConfig.h"
#include <stdint.h>

#ifndef BRYTEC_EMBEDDED
#include <filesystem>
#include <string>
#endif

struct EmptyString { };

class BinaryDeserializer {

public:
    BinaryDeserializer() = default;
    ~BinaryDeserializer();

    void setData(uint8_t* data, uint32_t length);

#ifndef BRYTEC_EMBEDDED
    void setDataFromPath(std::filesystem::path path);
#endif

    template <typename T>
    void readRaw(T* data)
    {
        readInternal((uint8_t*)data, sizeof(T));
    }

    bool readInternal(uint8_t* data, uint32_t dataSize);

private:
    uint8_t* m_data;
    uint32_t m_currentOffset = 0;
    bool m_ownData = false;
};

#ifndef BRYTEC_EMBEDDED
template <>
inline void BinaryDeserializer::readRaw<std::string>(std::string* data)
{
    uint32_t length;
    readRaw<uint32_t>(&length);

    if (length <= 0)
        return;

    data->resize(length + 1, '\0');

    for (int i = 0; i < length; i++)
        readRaw<char>(&data->data()[i]);
}
#endif

template <>
inline void BinaryDeserializer::readRaw<EmptyString>(EmptyString* data)
{
    uint32_t length;
    readRaw<uint32_t>(&length);

    if (length == 0)
        return;

    char c;
    for (uint32_t i = 0; i < length; i++)
        readRaw<char>(&c);
}