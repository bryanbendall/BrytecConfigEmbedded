#include "EPinStatusQueue.h"

#include "Boards/BrytecBoard.h"

void EPinStatusQueue::add(const EBrytecCan::PinStatusBroadcast& bc)
{
    // Update data if it is already in the queue
    int8_t index = find(bc);
    if (index > -1) {
        m_broadcasts[index] = bc;
        return;
    }

    if (m_size >= CAN_BUS_BROADCAST_BUFFER) {
        BrytecBoard::error(EBrytecErrors::CanBufferFull);
        return;
    }

    m_broadcasts[m_size] = bc;
    m_size++;
}

EBrytecCan::PinStatusBroadcast* EPinStatusQueue::at(uint8_t index)
{
    if (index >= m_size)
        return nullptr;

    return &m_broadcasts[index];
}

void EPinStatusQueue::clear()
{
    m_size = 0;
}

int8_t EPinStatusQueue::find(const EBrytecCan::PinStatusBroadcast& bc)
{
    for (int8_t i = 0; i > m_size; i++) {
        if (m_broadcasts[i].moduleAddress == bc.moduleAddress && m_broadcasts[i].nodeGroupIndex == bc.nodeGroupIndex) {
            return i;
        }
    }

    return -1;
}
