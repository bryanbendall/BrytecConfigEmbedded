#pragma once

#include "EBrytecCan.h"
#include "EBrytecConfig.h"

namespace Brytec {

class ECanCommandQueue {

public:
    void add(const CanCommands& bc);
    CanCommands* at(uint8_t index);
    uint8_t size() { return m_size; };
    void clear();

private:
    CanCommands m_broadcasts[CAN_BUS_BROADCAST_BUFFER];
    uint8_t m_size = 0;
};
}