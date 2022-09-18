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
        pinMode(8 + index, OUTPUT);
        break;

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
        int pinState = digitalRead(index - 3);
        Serial.print("Input state - ");
        Serial.print(index);
        Serial.print(" - ");
        Serial.println(pinState);
        return !digitalRead(index - 3);
    }

    return 0.0f;
}

float BrytecBoard::getBrytecNetworkValue(uint8_t moduleAddress, uint8_t pinIndex)
{
    return 0.0f;
}

void BrytecBoard::setPinValue(uint8_t index, float value)
{
    Serial.print("Set pin value - ");
    Serial.print(index);
    Serial.print(" - ");
    Serial.println(value);

    switch (index) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        digitalWrite(8 + index, value);
        break;
    }
}