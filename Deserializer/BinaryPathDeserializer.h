#pragma once

#include "BinaryDeserializer.h"

#if __has_include(<filesystem>)

#include <filesystem>
#include <vector>

namespace Brytec {

class BinaryPathDeserializer : public BinaryDeserializer {

public:
    BinaryPathDeserializer(std::filesystem::path path);
    ~BinaryPathDeserializer();
    uint32_t getTotalLength() { return m_dataLength; }
    void getRawData(uint8_t* dest, uint32_t offset, uint32_t length);

protected:
    bool readInternal(uint8_t* data, uint32_t dataSize) override;

private:
    std::vector<uint8_t> m_vec;
    uint32_t m_dataLength = 0;
};
}
#endif