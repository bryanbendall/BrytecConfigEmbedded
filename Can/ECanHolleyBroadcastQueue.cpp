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

void ECanHolleyBroadcastQueue::init(uint32_t size)
{
    reset();

    m_size = size;
    m_buffers[0] = (HolleyBroadcast*)malloc(sizeof(HolleyBroadcast) * size);
    m_buffers[1] = (HolleyBroadcast*)malloc(sizeof(HolleyBroadcast) * size);
}

void ECanHolleyBroadcastQueue::reset()
{
    free(m_buffers[0]);
    free(m_buffers[1]);
    m_buffers[0] = nullptr;
    m_buffers[1] = nullptr;
    m_size = 0;
}

void ECanHolleyBroadcastQueue::swapBuffers()
{
    m_writeButterIndex++;
    m_writeButterIndex = m_writeButterIndex % 2;
}

void ECanHolleyBroadcastQueue::insert(uint32_t index, const HolleyBroadcast& bc)
{
    if (!m_buffers[m_writeButterIndex])
        return;

    if (index >= m_size)
        return;

    m_buffers[m_writeButterIndex][index] = bc;
}

void ECanHolleyBroadcastQueue::update(const HolleyBroadcast& bc)
{
    if (!m_buffers[m_writeButterIndex])
        return;

    for (uint32_t i = 0; i < m_size; i++) {
        if (m_buffers[m_writeButterIndex][i].channel == bc.channel) {
            m_buffers[m_writeButterIndex][i] = bc;
            return;
        }
    }
}

float ECanHolleyBroadcastQueue::getValue(uint32_t index)
{
    if (index >= m_size)
        return 0.0f;

    // Get the buffer that is not being wrote to
    uint8_t buffIndex = m_writeButterIndex++;
    buffIndex = buffIndex % 2;

    if (!m_buffers[buffIndex])
        return 0.0f;

    return m_buffers[buffIndex][index].value;
}
}