#include "ECanHolleyBroadcastQueue.h"

#include <stdlib.h>
#include <string.h>

namespace Brytec {

void ECanHolleyBroadcastQueue::init(uint32_t size)
{
    reset();

    m_size = size;
    m_channels = (uint32_t*)malloc(sizeof(uint32_t) * size);
    m_buffers[0] = (CanFrame*)malloc(sizeof(CanFrame) * size);
    m_buffers[1] = (CanFrame*)malloc(sizeof(CanFrame) * size);

    // Clear buffers
    for (uint32_t i = 0; i < m_size; i++) {
        m_buffers[0][i] = CanFrame();
        m_buffers[1][i] = CanFrame();
    }
}

void ECanHolleyBroadcastQueue::reset()
{
    free(m_channels);
    free(m_buffers[0]);
    free(m_buffers[1]);
    m_channels = nullptr;
    m_buffers[0] = nullptr;
    m_buffers[1] = nullptr;
    m_size = 0;
}

void ECanHolleyBroadcastQueue::updateTimestep(uint32_t timestepMs)
{
    m_timeoutTime += timestepMs;

    // Clamp timeout at something big so we don't overflow
    if (m_timeoutTime >= 10000)
        m_timeoutTime = 10000;
}

void ECanHolleyBroadcastQueue::swapBuffers()
{
    // Get the buffer that is not being wrote to and clear it
    uint8_t buffIndex = m_writeButterIndex + 1;
    buffIndex = buffIndex % 2;
    for (uint32_t i = 0; i < m_size; i++)
        m_buffers[buffIndex][i] = CanFrame();

    m_writeButterIndex++;
    m_writeButterIndex = m_writeButterIndex % 2;
}

void ECanHolleyBroadcastQueue::insert(uint32_t index, uint32_t channel)
{
    if (index >= m_size)
        return;

    if (!m_channels)
        return;

    m_channels[index] = channel;
}

void ECanHolleyBroadcastQueue::update(const CanFrame& frame)
{
    // Not from ecu
    if (((frame.id >> 11) & 0x07) != 0x02)
        return;

    // Reset timeout when we get a new message
    m_timeoutTime = 0;

    if (!m_buffers[m_writeButterIndex])
        return;

    uint32_t frameChannel = getChannel(frame);
    if (frameChannel == UINT32_MAX)
        return;

    for (uint32_t i = 0; i < m_size; i++) {
        if (m_channels[i] == frameChannel) {
            m_buffers[m_writeButterIndex][i] = frame;
            return;
        }
    }
}

CanFrame ECanHolleyBroadcastQueue::getFrame(uint32_t index) const
{
    if (index >= m_size)
        return CanFrame();

    // Get the buffer that is not being wrote to
    uint8_t buffIndex = m_writeButterIndex + 1;
    buffIndex = buffIndex % 2;

    return m_buffers[buffIndex][index];
}

bool ECanHolleyBroadcastQueue::isDataValid() const
{
    return m_timeoutTime < 500;
}

uint32_t ECanHolleyBroadcastQueue::getChannel(const CanFrame& frame)
{
    // Not the right format
    if (frame.type != CanFrameType::Ext)
        return UINT32_MAX;

    // Not from ecu
    if (((frame.id >> 11) & 0x07) != 0x02)
        return UINT32_MAX;

    return (frame.id & 0x1FFC000) >> 14;
}
}