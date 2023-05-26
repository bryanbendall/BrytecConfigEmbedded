#pragma once

#include "EBrytecConfig.h"
#include <stdint.h>
#include <string.h>

#if __has_include(<string>)
#include <string>
#endif

namespace Brytec {

struct EmptyString { };

class BinaryDeserializer {

public:
    template <typename T>
    void readRaw(T* data)
    {
        if (!readInternal((uint8_t*)data, sizeof(T))) {
            // Set to 0 if it doesn't rear correctly
            memset(data, 0, sizeof(T));
        }
    }

    virtual bool readInternal(uint8_t* data, uint32_t dataSize) = 0;
    uint32_t getCurrentOffset() { return m_currentOffset; }

protected:
    BinaryDeserializer() = default;
    ~BinaryDeserializer() = default;

protected:
    uint32_t m_currentOffset = 0;
};

#if __has_include(<string>)
template <>
inline void BinaryDeserializer::readRaw<std::string>(std::string* data)
{
    uint32_t length;
    readRaw<uint32_t>(&length);

    if (length <= 0)
        return;

    data->resize(length, ' ');

    for (uint32_t i = 0; i < length; i++)
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

}