#pragma once

#include "EBrytecCan.h"
#include "EBrytecConfig.h"

namespace Brytec {

struct HolleyBroadcast {
    uint32_t channel = UINT32_MAX;
    float value = 0.0f;

    HolleyBroadcast() = default;
    HolleyBroadcast(uint32_t channleNumber);
    HolleyBroadcast(const CanFrame& frame);
    explicit operator bool() const { return channel != UINT32_MAX; }
};

class ECanHolleyBroadcastQueue {

public:
    ECanHolleyBroadcastQueue() = default;
    ECanHolleyBroadcastQueue(ECanHolleyBroadcastQueue& other) = delete;

    void init(uint32_t size);
    void reset();
    void swapBuffers();
    void insert(uint32_t index, const HolleyBroadcast& bc);
    void update(const HolleyBroadcast& bc);
    uint32_t getSize() { return m_size; }
    float getValue(uint32_t index);

private:
    HolleyBroadcast* m_buffers[2];
    uint8_t m_writeButterIndex = 0;
    uint32_t m_size = 0;
};
}