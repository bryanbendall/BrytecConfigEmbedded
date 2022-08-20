#pragma once

struct EmptyString { };

// #define Embedded
#ifndef Embedded
#include <filesystem>
#endif

class BinaryDeserializer {

public:
    BinaryDeserializer(uint8_t* data);
#ifndef Embedded
    BinaryDeserializer(std::filesystem::path path);
#endif

    ~BinaryDeserializer();

    template <typename T>
    T readRaw()
    {
        uint64_t temp = 0;
        for (int i = m_currentOffset; i < m_currentOffset + sizeof(T); i++) {
            temp |= (uint64_t)m_data[i] << ((i - m_currentOffset) * 8);
        }
        m_currentOffset += sizeof(T);
        return *((T*)&temp);
    }

    void saveOffset() { m_savedOffset = m_currentOffset; }
    void goToSavedOffset() { m_currentOffset = m_savedOffset; }

private:
    bool m_ownData = false;
    uint8_t* m_data;
    uint64_t m_currentOffset = 0;
    uint64_t m_savedOffset = 0;
};

#ifndef Embedded
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
inline EmptyString BinaryDeserializer::readRaw<EmptyString>()
{
    uint32_t length = readRaw<uint32_t>();

    if (length <= 0)
        return {};

    char data[length];

    for (int i = 0; i < length; i++)
        data[i] = readRaw<char>();

    return {};
}