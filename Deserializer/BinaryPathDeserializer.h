#pragma once

#include "BinaryDeserializer.h"

#if __has_include(<filesystem>)

#include <filesystem>

namespace Brytec {

class BinaryPathDeserializer : public BinaryDeserializer {

public:
    BinaryPathDeserializer(std::filesystem::path path);
    ~BinaryPathDeserializer();

protected:
    bool readInternal(uint8_t* data, uint32_t dataSize) override;

private:
    const uint8_t* m_data;
    uint32_t m_dataLength = 0;
};
}
#endif