#include "BinaryDeserializer.h"

#include "EBrytecConfig.h"

static uint32_t s_currentOffset = 0;

static uint8_t* s_data;

bool BinaryDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    if (data) {
        memcpy(data, s_data[s_currentOffset], dataSize);
        s_currentOffset += dataSize;
        return true;
    } else {
        return false;
    }
}