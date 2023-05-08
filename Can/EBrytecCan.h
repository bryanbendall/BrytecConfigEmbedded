#pragma once

#include <stdint.h>

namespace Brytec {

struct CanExtFrame {
    uint32_t id = 0;
    uint8_t dlc = 8;
    uint8_t data[8] = {};

    bool isBroadcast() const;
    explicit operator bool() const { return id > 0; }
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

struct CanCommands {

    static constexpr uint8_t AllModules = 0xFF;
    static constexpr uint16_t NoNodeGroup = 0xFFFF;

    enum Command : uint8_t {
        Nak,
        Ack,
        ChangeMode,
        ReloadConfig,
        RequestStatus,
        SendStatus,
        ReserveConfigSize,
        WriteConfigData,
        ReadConfigRequest,
        ReadConfigData
    };

    Command command;
    uint8_t moduleAddress = AllModules;
    uint16_t nodeGroupIndex = NoNodeGroup;
    uint8_t data[8] = {};

    CanCommands() = default;
    CanCommands(const CanExtFrame& frame);
    CanExtFrame getFrame();
};
}