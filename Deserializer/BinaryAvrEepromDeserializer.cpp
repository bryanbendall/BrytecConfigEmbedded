#include "BinaryAvrEepromDeserializer.h"

#include <string.h>
#include <avr/eeprom.h>

namespace Brytec {

BinaryAvrEepromDeserializer::BinaryAvrEepromDeserializer()
{
    m_currentOffset = sizeof(uint32_t);
}

uint32_t BinaryAvrEepromDeserializer::getLength()
{
    return eeprom_read_dword(0);
}

void BinaryAvrEepromDeserializer::setLength(uint32_t length)
{
    eeprom_update_dword(0, length);
}

bool BinaryAvrEepromDeserializer::readInternal(uint8_t* data, uint32_t dataSize)
{
    if (data) {
        eeprom_read_block(data, (const void*)m_currentOffset, dataSize);
        m_currentOffset += dataSize;
        return true;
    } else {
        return false;
    }
}
}