#pragma once

#include <stdint.h>

namespace EBrytecCan {

struct CanExtFrame {
    uint32_t id;
    uint8_t data[8];
};

static bool isBroadcast(const CanExtFrame& frame);

struct PinStatusBroadcast {

    enum StatusFlags : uint8_t {
        DISABLED,
        NORMAL,
        TRIPPED
    };

    StatusFlags statusFlags = StatusFlags::DISABLED; // 4 bits
    uint8_t moduleAddress = 0;
    uint16_t nodeGroupIndex = 0;
    float current = 0.0f; // 2 decimal places
    float voltage = 0.0f; // 2 decimal places
    float value = 0.0f;

    PinStatusBroadcast() = default;
    PinStatusBroadcast(const CanExtFrame& frame);
    CanExtFrame getFrame();
};
};