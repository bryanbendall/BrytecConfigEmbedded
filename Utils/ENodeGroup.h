#pragma once

#include "IOTypes.h"
#include <stdint.h>

namespace Brytec {

#define TrippedTime 100 // ms

struct ENodeGroup {
    bool enabled;
    bool usedOnBus;
    uint16_t index;
    IOTypes::Types type;
    uint8_t nodeCount;
    uint8_t currentLimit;
    bool alwaysRetry;
    uint8_t maxRetries;
    uint32_t retryDelay;
    uint32_t startNodeIndex;
    uint64_t uuid;

    uint32_t trippedTimer = 0;
    uint32_t retryTimer = 0;
    bool tripped = false;
    uint8_t numberRetries = 0;

    void setupPin();
    void updateInitialValue();
    void updatePreviousValueNodes();
    void updateFinalValue();
    void updatePinCurrent(uint32_t timestepMs);
    void checkOverCurrent(uint32_t timestepMs, float current);
    float getFinalValue();
};

}