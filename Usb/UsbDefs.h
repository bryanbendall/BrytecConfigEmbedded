#pragma once

#include <stdint.h>

namespace Brytec {

constexpr uint8_t PacketStart = 0xFF;

enum class UsbPacketType : uint8_t {
    Unknown = 0,
    Command,
    Can,
    Status,
    DebugPrint
};

struct UsbPacket {
public:
    static constexpr uint8_t bufferSize = 62;

    uint8_t length = 0;
    uint8_t data[bufferSize] = {};

    explicit operator bool() const { return length > 0; }

    UsbPacketType getType();

    template <typename T>
    void set(const T& t);

    template <typename T>
    T as() const;
};

}