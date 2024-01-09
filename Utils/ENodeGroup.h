#pragma once

#include "IOTypes.h"
#include <stdint.h>

namespace Brytec {

#define TrippedTime 0.1f

struct ENodeGroup {
    bool enabled;
    bool usedOnBus;
    uint16_t index;
    IOTypes::Types type;
    uint8_t nodeCount;
    uint8_t currentLimit;
    bool alwaysRetry;
    uint8_t maxRetries;
    float retryDelay;
    uint32_t startNodeIndex;

    float trippedTimer = 0.0f;
    float retryTimer = 0.0f;
    bool tripped = false;
    uint8_t numberRetries = 0;

    void setupPin();
    void updateInitialValue();
    void updateFinalValue();
    void updatePinCurrent(float timestep);
    void checkOverCurrent(float timestep, float current);
    float getFinalValue();
};

}