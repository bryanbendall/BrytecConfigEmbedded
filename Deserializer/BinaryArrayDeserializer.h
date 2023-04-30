#pragma once

#include "BinaryDeserializer.h"

namespace Brytec {

class BinaryArrayDeserializer : public BinaryDeserializer {

public:
    BinaryArrayDeserializer(const uint8_t* data, uint32_t length);

protected:
    virtual bool readInternal(uint8_t* data, uint32_t dataSize) override;

private:
    const uint8_t* m_data;
    uint32_t m_dataLength;
};
}