#pragma once

#include "Deserializer/BinaryDeserializer.h"

#if __has_include(<avr/eeprom.h>)

namespace Brytec {

class BinaryAvrEepromDeserializer : public BinaryDeserializer {

public:
    BinaryAvrEepromDeserializer();
    uint32_t getLength();
    void setLength(uint32_t length);

protected:
    virtual bool readInternal(uint8_t* data, uint32_t dataSize) override;

};
}

#endif