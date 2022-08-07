#pragma once

#include "BrytecConfigEmbedded/IOTypes.h"
#include "BrytecConfigEmbedded/Utils/EBrytecErrors.h"
#include <stdint.h>

class BrytecBoard {

public:
    static void error(EBrytecErrors error);
    static void setupPin(uint8_t index, IOTypes::Types type);
    static float getPinValue(uint8_t index);
    static void setPinValue(uint8_t index, float value);
};