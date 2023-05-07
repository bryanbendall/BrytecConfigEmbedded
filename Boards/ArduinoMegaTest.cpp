#include "BrytecBoard.h"
#include <Arduino.h>
#include <CAN.h>
#include <avr/pgmspace.h>

#include "Program.h"

namespace Brytec {

Brytec::BinaryProgmemDeserializer des(progmem_data, sizeof(progmem_data));

BinaryDeserializer* BrytecBoard::getDeserializer()
{
    return &des;
}

void BrytecBoard::error(EBrytecErrors error)
{
    switch (error) {
    case EBrytecErrors::ModuleHeader:
        Serial.println(F("Module header is wrong"));
        break;
    case EBrytecErrors::NodeGroupHeader:
        Serial.println(F("Node Group header is wrong"));
        break;
    case EBrytecErrors::ModuleNotEnabled:
        Serial.println(F("This module is not enabled"));
        break;
    case EBrytecErrors::AddNodeFailed:
        Serial.println(F("Failed to add node"));
        break;
    case EBrytecErrors::NodeVectorOutOfSpace:
        Serial.println(F("Node Vector out of space"));
        break;
    case EBrytecErrors::FailedToCreateNode:
        Serial.println(F("Failed to create node"));
        break;
    case EBrytecErrors::NodeIndexOutOfBounds:
        Serial.println(F("Node index out of bounds"));
        break;
    case EBrytecErrors::BadAlloc:
        Serial.println(F("Node group allocation failed"));
        break;
    case EBrytecErrors::CanBufferFull:
        Serial.println(F("Can Buffer Full"));
        break;
    default:
        Serial.println(F("Unknown error!"));
        break;
    }
}

void BrytecBoard::setupBrytecCan(uint32_t mask, uint32_t filter)
{
    CAN.setPins(PIN_SPI_SS, 42);

    // start the CAN bus at 500 kbps
    if (!CAN.begin(500E3)) {
        Serial.println("Starting CAN failed!");
        while (1)
            ;
    } else {
        Serial.println("Starting can GOOD");
    }
}

void BrytecBoard::setupPin(uint16_t index, IOTypes::Types type)
{
    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        pinMode(1 + index, OUTPUT);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        pinMode(2 + index, OUTPUT);
        break;

    case 12:
    case 13:
    case 14:
    case 15:
        Serial.print("setting input - ");
        Serial.println(index);
        pinMode(index - 12 + A0, INPUT_PULLUP);
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        Serial.print("setting input - ");
        Serial.println(index);
        pinMode(index - 16 + A8, INPUT_PULLUP);
        break;
    }
}

void BrytecBoard::shutdownAllPins()
{
    for (int i = 0; i < 12; i++)
        setPinValue(i, IOTypes::Types::Undefined, 0.0f);
}

float BrytecBoard::getPinValue(uint16_t index)
{
    switch (index) {
    case 12:
    case 13:
    case 14:
    case 15:
        return !digitalRead(index - 12 + A0);
    case 16:
    case 17:
    case 18:
    case 19:
        return !digitalRead(index - 16 + A8);
    }

    return 0.0f;
}

float BrytecBoard::getPinVoltage(uint16_t index)
{
    return 0.0f;
}

float BrytecBoard::getPinCurrent(uint16_t index)
{
    return 0.0f;
}

void BrytecBoard::setPinValue(uint16_t index, IOTypes::Types type, float value)
{
    int pwmValue = (value * 255.0f);

    if (pwmValue > 255)
        pwmValue = 255;

    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        analogWrite(1 + index, pwmValue);
        break;
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        analogWrite(2 + index, pwmValue);
        break;
    }
}

void BrytecBoard::sendBrytecCan(CanExtFrame frame)
{
    CAN.beginExtendedPacket(frame.id);
    CAN.write(frame.data, 8);
    CAN.endPacket();
}

void BrytecBoard::updateConfig(uint8_t* data, uint32_t size, uint32_t offset)
{
    // Not supported with progmem
}
}