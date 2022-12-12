#pragma once

#include "EBrytecCan.h"
#include "EBrytecConfig.h"

class EPinStatusQueue {

public:
    void add(const EBrytecCan::PinStatusBroadcast& bc);
    EBrytecCan::PinStatusBroadcast* at(uint8_t index);
    uint8_t size() { return m_size; };
    void clear();

private:
    int8_t find(const EBrytecCan::PinStatusBroadcast& bc);

private:
    EBrytecCan::PinStatusBroadcast m_broadcasts[CAN_BUS_BROADCAST_BUFFER];
    uint8_t m_size = 0;
};