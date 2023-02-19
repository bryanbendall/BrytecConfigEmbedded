#pragma once

#include "Can/EBrytecCan.h"
#include "IOTypes.h"
#include "Utils/EBrytecErrors.h"
#include <stdint.h>

class BrytecBoard {

public:
    static void error(EBrytecErrors error);
    static void setupBrytecCan(uint8_t moduleAddress);
    static void setCanMaskAndFilter(uint8_t index, uint32_t mask, uint32_t filter);
    static void setupPin(uint8_t index, IOTypes::Types type);
    static float getPinValue(uint8_t index);
    static float getPinVoltage(uint8_t index);
    static float getPinCurrent(uint8_t index);
    static void setPinValue(uint8_t index, IOTypes::Types type, float value);
    static void sendBrytecCan(EBrytecCan::CanExtFrame frame);
};