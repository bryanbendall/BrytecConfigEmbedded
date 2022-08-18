#pragma once

#include "IOTypes.h"
#include <stdint.h>

struct ENodeGroup {
    uint32_t startNodeIndex;
    uint8_t nodeCount;
    uint8_t boardPinIndex;
    bool enabled;
    IOTypes::Types type;

    void setupPin();
    void updateInitialValue();
    void updateFinalValue();
};