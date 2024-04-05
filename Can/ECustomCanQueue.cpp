#include "ECustomCanQueue.h"

#include <stdlib.h>
#include <string.h>

namespace Brytec {

void ECustomCanQueue::init(uint32_t size)
{
    reset();

    m_size = size;
    m_addresses = (uint32_t*)malloc(sizeof(uint32_t) * size);
    m_buffers[0] = (CanFrame*)malloc(sizeof(CanFrame) * size);
    m_buffers[1] = (CanFrame*)malloc(sizeof(CanFrame) * size);
}

void ECustomCanQueue::reset()
{
    free(m_addresses);
    free(m_buffers[0]);
    free(m_buffers[1]);
    m_addresses = nullptr;
    m_buffers[0] = nullptr;
    m_buffers[1] = nullptr;
    m_size = 0;
}

void ECustomCanQueue::swapBuffers()
{
    // Get the buffer that is not being wrote to and clear it
    uint8_t buffIndex = m_writeButterIndex + 1;
    buffIndex = buffIndex % 2;
    for (uint32_t i = 0; i < m_size; i++) {
        m_buffers[buffIndex][i] = CanFrame();
    }

    m_writeButterIndex++;
    m_writeButterIndex = m_writeButterIndex % 2;
}

void ECustomCanQueue::insert(uint32_t index, const CanFrame& frame)
{
    if (index >= m_size)
        return;

    if (!m_addresses)
        return;

    m_addresses[index] = frame.id;
}

void ECustomCanQueue::update(const CanFrame& frame)
{
    if (!m_buffers[m_writeButterIndex])
        return;

    for (uint32_t i = 0; i < m_size; i++) {

        if(m_addresses[i] == frame.id){
            m_buffers[m_writeButterIndex][i] = frame;
            return;
        }
    }
}

CanFrame ECustomCanQueue::getFrame(uint32_t index)
{
    if (index >= m_size)
        return CanFrame();

    // Get the buffer that is not being wrote to
    uint8_t buffIndex = m_writeButterIndex + 1;
    buffIndex = buffIndex % 2;

    return m_buffers[buffIndex][index];
}
}