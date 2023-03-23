#pragma once

#include "IOTypes.h"
#include <stdint.h>

namespace Brytec {

#define TrippedTime 0.1f

struct ENodeGroup {
    bool enabled;
    bool usedOnBus;
    uint16_t boardPinIndex;
    IOTypes::Types type;
    uint32_t startNodeIndex;
    uint8_t nodeCount;
    uint8_t currentLimit;
    bool alwaysRetry;
    uint8_t maxRetries;
    float retryDelay;

    bool tripped = false;
    float trippedTimer = 0.0f;
    float retryTimer = 0.0f;
    uint8_t numberRetries = 0;

    void setupPin();
    void updateInitialValue();
    void updateFinalValue();
    void updatePinCurrent(float timestep);
    void checkOverCurrent(float timestep, float current);
    float getFinalValue();
};

}