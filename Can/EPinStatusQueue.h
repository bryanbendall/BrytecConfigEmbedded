#pragma once

#include "EBrytecCan.h"
#include "EBrytecConfig.h"

namespace Brytec {

class EPinStatusQueue {

public:
    void add(const PinStatusBroadcast& bc);
    PinStatusBroadcast* at(uint8_t index);
    uint8_t size() { return m_size; };
    void clear();

private:
    int8_t find(const PinStatusBroadcast& bc);

private:
    PinStatusBroadcast m_broadcasts[CAN_BUS_BROADCAST_BUFFER];
    uint8_t m_size = 0;
};

}