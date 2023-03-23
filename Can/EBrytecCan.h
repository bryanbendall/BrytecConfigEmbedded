#pragma once

#include <stdint.h>

namespace Brytec {

struct CanExtFrame {
    uint32_t id = 0;
    uint8_t dlc = 8;
    uint8_t data[8];

    bool isBroadcast() const;
};

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

}