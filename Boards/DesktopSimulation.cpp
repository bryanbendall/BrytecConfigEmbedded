#include "BrytecBoard.h"

#include <iostream>

void BrytecBoard::error(EBrytecErrors error)
{
    switch (error) {
    case EBrytecErrors::ModuleHeader:
        std::cout << "Module header is wrong" << std::endl;
        break;
    case EBrytecErrors::NodeGroupHeader:
        std::cout << "Node Group header is wrong" << std::endl;
        break;
    case EBrytecErrors::NotEnabled:
        std::cout << "This module is not enabled" << std::endl;
        break;
    case EBrytecErrors::AddNodeFailed:
        std::cout << "Failed to add node" << std::endl;
        break;
    case EBrytecErrors::NodeVectorOutOfSpace:
        std::cout << "Node Vector out of space" << std::endl;
        break;
    case EBrytecErrors::FailedToCreateNode:
        std::cout << "Failed to create node" << std::endl;
        break;
    case EBrytecErrors::NodeIndexOutOfBounds:
        std::cout << "Node index out of bounds" << std::endl;
        break;
    case EBrytecErrors::BadAlloc:
        std::cout << "Node group allocation failed" << std::endl;
        break;

    default:
        break;
    }
}

void BrytecBoard::setupBrytecCan(uint8_t moduleAddress)
{
}

void BrytecBoard::setupPin(uint8_t index, IOTypes::Types type)
{
}

float BrytecBoard::getPinValue(uint8_t index)
{
    return 0.0f;
}

float BrytecBoard::getPinCurrent(uint8_t index)
{
    return 0.0f;
}

float BrytecBoard::getBrytecNetworkValue(uint8_t moduleAddress, uint8_t pinIndex)
{
    return 0.0f;
}

void BrytecBoard::setPinValue(uint8_t index, IOTypes::Types type, float value)
{
}