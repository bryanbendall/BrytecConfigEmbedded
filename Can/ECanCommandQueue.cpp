#include "ECanCommandQueue.h"

#include "Boards/BrytecBoard.h"

namespace Brytec {

void ECanCommandQueue::add(const CanCommands& bc)
{
    if (m_size >= CAN_BUS_BROADCAST_BUFFER) {
        BrytecBoard::error(EBrytecErrors::CanBufferFull);
        return;
    }

    m_broadcasts[m_size] = bc;
    m_size++;
}

CanCommands* ECanCommandQueue::at(uint8_t index)
{
    if (index >= m_size)
        return nullptr;

    return &m_broadcasts[index];
}

void ECanCommandQueue::clear()
{
    m_size = 0;
}

}