#pragma once

#include "IOTypes.h"
#include "Utils/EBrytecErrors.h"
#include <stdint.h>

class BrytecBoard {

public:
    static void error(EBrytecErrors error);
    static void setupBrytecCan(uint8_t moduleAddress);
    static void setupPin(uint8_t index, IOTypes::Types type);
    static float getPinValue(uint8_t index);
    static float getBrytecNetworkValue(uint8_t moduleAddress, uint8_t pinIndex);
    static void setPinValue(uint8_t index, float value);
};