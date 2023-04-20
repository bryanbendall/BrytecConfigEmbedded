#pragma once

#include <stdint.h>

namespace Brytec {

enum class UsbPacketType : uint8_t {
    Unknown = 0,
    Command,
    Can,
    Status
};

struct UsbPacket {
    uint32_t length = 0;
    uint8_t data[64] = {};

    UsbPacketType getType();

    template <typename T>
    void set(T& t);

    template <typename T>
    T as();
};

}