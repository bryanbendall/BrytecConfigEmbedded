#include "BrytecBoard.h"
#include <Arduino.h>
#include <CAN.h>

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
    CAN.setPins(PIN_SPI_SS, 8);

    // start the CAN bus at 500 kbps
    if (!CAN.begin(500E3)) {
        Serial.println(F("Starting CAN failed!"));
        while (1)
            ;
    } else {
        Serial.println(F("Starting can GOOD"));
    }
}

void BrytecBoard::setupPin(uint8_t index, IOTypes::Types type)
{
    switch (index) {
        // case 0:
        // case 1:
        // case 2:
        // case 3:
        // case 4:
        // case 5:
        //     pinMode(8 + index, OUTPUT);
        //     break;

    case 6:
    case 7:
    case 8:
    case 9:
        pinMode(index - 3, INPUT_PULLUP);
        break;
    }
}

float BrytecBoard::getPinValue(uint8_t index)
{
    switch (index) {
    case 6:
    case 7:
    case 8:
    case 9:
        return !digitalRead(index - 3);
    }

    return 0.0f;
}

float BrytecBoard::getPinVoltage(uint8_t index)
{
    return 0.0f;
}

float BrytecBoard::getPinCurrent(uint8_t index)
{
    return 0.0f;
}

void BrytecBoard::setPinValue(uint8_t index, IOTypes::Types type, float value)
{
    int pwmValue = (value * 255.0f);

    if (pwmValue > 255)
        pwmValue = 255;

    // switch (index) {
    // case 0:
    // case 1:
    // case 2:
    // case 3:
    // case 4:
    // case 5:
    //     analogWrite(8 + index, pwmValue);
    //     break;
    // }
}

void BrytecBoard::sendBrytecCan(EBrytecCan::CanExtFrame frame)
{
    CAN.beginExtendedPacket(frame.id);
    CAN.write(frame.data, 8);
    CAN.endPacket();
}