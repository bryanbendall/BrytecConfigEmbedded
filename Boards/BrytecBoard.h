#pragma once

#include "Can/EBrytecCan.h"
#include "Can/ECanBus.h"
#include "Deserializer/BinaryDeserializer.h"
#include "IOTypes.h"
#include "Utils/EBrytecErrors.h"
#include <stdint.h>

namespace Brytec {

class BrytecBoard {

public:
    static BinaryDeserializer* getDeserializer();
    static void preUpdate(uint32_t timestepMs);
    static void postUpdate(uint32_t timestepMs);
    static void error(EBrytecErrors error);
    static void setupCan(uint8_t index, CanSpeed::Types speed);
    static void setupPin(uint16_t index, IOTypes::Types type);
    static void shutdownAllPins();
    static float getPinValue(uint16_t index, IOTypes::Types type);
    static float getPinVoltage(uint16_t index);
    static float getPinCurrent(uint16_t index);
    static void setPinValue(uint16_t index, IOTypes::Types type, float value);
    static void sendCan(uint8_t index, const CanFrame& frame);
    static void sendBrytecCanUsb(const CanFrame& frame);
    static void ReserveConfigSize(uint16_t size);
    static void updateConfig(uint8_t* data, uint32_t size, uint32_t offset);
    static uint32_t getTemplateSize();
    static void getTemplateData(uint8_t* dest, uint32_t offset, uint32_t length);
    static uint32_t getConfigSize();
    static void getConfigData(uint8_t* dest, uint32_t offset, uint32_t length);
#if __has_include(<string>)
    static void AddedNamesNodeGroup(uint16_t index, std::string ngName);
#endif
};

}