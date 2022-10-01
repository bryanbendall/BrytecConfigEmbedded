#include "BinaryDeserializer.h"

#include "EBrytecConfig.h"
#include <Arduino.h>

#include "Deserializer/Program.h"

static uint32_t s_currentOffset = 0;

BinaryDeserializer::~BinaryDeserializer()
{
}

bool BinaryDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    memcpy_P(data, &progmem_data[s_currentOffset], dataSize);
    s_currentOffset += dataSize;
    return true;
}

void setData(uint8_t* data) { }