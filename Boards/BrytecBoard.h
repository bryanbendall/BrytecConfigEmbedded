#pragma once

#include "Can/EBrytecCan.h"
#include "Deserializer/BinaryDeserializer.h"
#include "IOTypes.h"
#include "Utils/EBrytecErrors.h"
#include <stdint.h>

namespace Brytec {

class BrytecBoard {

public:
    static BinaryDeserializer* getDeserializer();
    static void error(EBrytecErrors error);
    static void setupBrytecCan(uint32_t mask, uint32_t filter);
    static void setupPin(uint16_t index, IOTypes::Types type);
    static void shutdownAllPins();
    static float getPinValue(uint16_t index, IOTypes::Types type);
    static float getPinVoltage(uint16_t index);
    static float getPinCurrent(uint16_t index);
    static void setPinValue(uint16_t index, IOTypes::Types type, float value);
    static void sendBrytecCan(const CanExtFrame& frame);
    static void sendBrytecCanUsb(const CanExtFrame& frame);
    static void ReserveConfigSize(uint16_t size);
    static void updateConfig(uint8_t* data, uint32_t size, uint32_t offset);
    static uint32_t getTemplateSize();
    static void getTemplateData(uint8_t* dest, uint32_t offset, uint32_t length);
    static uint32_t getConfigSize();
    static void getConfigData(uint8_t* dest, uint32_t offset, uint32_t length);
};

}