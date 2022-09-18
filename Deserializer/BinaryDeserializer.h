#pragma once

#include "EBrytecConfig.h"
#include <stdint.h>

struct EmptyString { };

class BinaryDeserializer {

public:
    BinaryDeserializer() = default;

    template <typename T>
    void readRaw(const T* data)
    {
        readInternal((uint8_t*)data, sizeof(T));
    }

    bool readInternal(uint8_t* data, uint32_t dataSize);

};

#ifndef BRYTEC_EMBEDDED
template <>
inline std::string BinaryDeserializer::readRaw<std::string>()
{
    uint32_t length = readRaw<uint32_t>();

    if (length <= 0)
        return "";

    char data[length];

    for (int i = 0; i < length; i++)
        data[i] = readRaw<char>();

    return std::string(data, length);
}
#endif

template <>
inline void BinaryDeserializer::readRaw<EmptyString>(const EmptyString* data)
{
    uint32_t length;
    readRaw<uint32_t>(&length);

    if (length <= 0)
        return;

    char c;
    for (uint32_t i = 0; i < length; i++)
        readRaw<char>(&c);
}