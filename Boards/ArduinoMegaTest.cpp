#include "BrytecBoard.h"
#include <Arduino.h>

void BrytecBoard::error(EBrytecErrors error)
{
    switch (error) {
    case EBrytecErrors::ModuleHeader:
        Serial.println("Module header is wrong");
        break;
    case EBrytecErrors::NodeGroupHeader:
        Serial.println("Node Group header is wrong");
        break;
    case EBrytecErrors::NotEnabled:
        Serial.println("This module is not enabled");
        break;
    case EBrytecErrors::AddNodeFailed:
        Serial.println("Failed to add node");
        break;
    case EBrytecErrors::NodeVectorOutOfSpace:
        Serial.println("Node Vector out of space");
        break;
    case EBrytecErrors::FailedToCreateNode:
        Serial.println("Failed to create node");
        break;
    case EBrytecErrors::NodeIndexOutOfBounds:
        Serial.println("Node index out of bounds");
        break;
    case EBrytecErrors::BadAlloc:
        Serial.println("Node group allocation failed");
        break;
    default:
        Serial.println("Unknown error!");
        break;
    }
}

void BrytecBoard::setupBrytecCan(uint8_t moduleAddress)
{
}

void BrytecBoard::setupPin(uint8_t index, IOTypes::Types type)
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

float BrytecBoard::getPinValue(uint8_t index)
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
    int pwmValue = (value * 2.55f);

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

void BrytecBoard::sendBrytecCan(const EBrytecCan::CanExtFrame& frame)
{
}