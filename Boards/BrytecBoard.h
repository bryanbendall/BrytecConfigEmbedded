#pragma once

#include "Can/EBrytecCan.h"
#include "IOTypes.h"
#include "Utils/EBrytecErrors.h"
#include <stdint.h>

namespace Brytec {

class BrytecBoard {

public:
    static void error(EBrytecErrors error);
    static void setupBrytecCan(uint32_t mask, uint32_t filter);
    static void setupPin(uint16_t index, IOTypes::Types type);
    static float getPinValue(uint16_t index);
    static float getPinVoltage(uint16_t index);
    static float getPinCurrent(uint16_t index);
    static void setPinValue(uint16_t index, IOTypes::Types type, float value);
    static void sendBrytecCan(CanExtFrame frame);
};

}