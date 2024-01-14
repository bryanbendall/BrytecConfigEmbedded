#include "ECanHolleyBroadcastQueue.h"

#include <stdlib.h>
#include <string.h>

namespace Brytec {

HolleyBroadcast::HolleyBroadcast(uint32_t channleNumber)
    : channel(channleNumber)
{
}

HolleyBroadcast::HolleyBroadcast(const CanFrame& frame)
{
    // Not the right format
    if (frame.type != CanFrameType::Ext)
        return;

    // Not from ecu
    if (((frame.id >> 11) & 0x07) != 0x02)
        return;

    channel = (frame.id & 0x1FFC000) >> 14;

    // Convert from big endian to small endian
    uint8_t* v = (uint8_t*)&value;
    v[0] = frame.data[3];
    v[1] = frame.data[2];
    v[2] = frame.data[1];
    v[3] = frame.data[0];
}

ECanHolleyBroadcastQueue::ECanHolleyBroadcastQueue(ECanHolleyBroadcastQueue& other)
{
    other.init(m_size);
    memcpy(other.m_broadcasts, m_broadcasts, sizeof(HolleyBroadcast) * m_size);
}

void ECanHolleyBroadcastQueue::init(uint32_t size)
{
    reset();

    m_size = size;
    m_broadcasts = (HolleyBroadcast*)malloc(sizeof(HolleyBroadcast) * size);
}

void ECanHolleyBroadcastQueue::reset()
{
    free(m_broadcasts);
    m_broadcasts = nullptr;
    m_size = 0;
}

void ECanHolleyBroadcastQueue::insert(uint32_t index, const HolleyBroadcast& bc)
{
    if (!m_broadcasts)
        return;

    if (index >= m_size)
        return;

    m_broadcasts[index] = bc;
}

void ECanHolleyBroadcastQueue::update(const HolleyBroadcast& bc)
{
    if (!m_broadcasts)
        return;

    for (int i = 0; i < m_size; i++) {
        if (m_broadcasts[i].channel == bc.channel) {
            m_broadcasts[i] = bc;
            return;
        }
    }
}

float ECanHolleyBroadcastQueue::getValue(uint32_t index)
{
    if (index >= m_size)
        return 0.0f;

    return m_broadcasts[index].value;
}
}