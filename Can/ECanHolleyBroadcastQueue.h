#pragma once

#include "EBrytecCan.h"
#include "EBrytecConfig.h"

namespace Brytec {

class ECanHolleyBroadcastQueue {

public:
    ECanHolleyBroadcastQueue() = default;
    ECanHolleyBroadcastQueue(ECanHolleyBroadcastQueue& other) = delete;

    void init(uint32_t size);
    void reset();
    void swapBuffers();
    void insert(uint32_t index, uint32_t channel);
    void update(const CanFrame& frame);
    uint32_t getSize() { return m_size; }
    CanFrame getFrame(uint32_t index);

private:
    uint32_t getChannel(const CanFrame& frame);

private:
    uint32_t* m_channels = nullptr;
    CanFrame* m_buffers[2] = { nullptr };
    uint8_t m_writeButterIndex = 0;
    uint32_t m_size = 0;
};
}