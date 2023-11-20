#pragma once

#include <stdint.h>

namespace Brytec {

struct CanTypes {
    enum class Types : uint8_t {
        Disabled,
        Brytec,
        Custom,
        Count
    };

    static const char* Strings[(size_t)Types::Count];
    static const char* getString(Types type) { return Strings[(int)type]; }
};

struct CanSpeed {
    enum class Types : uint8_t {
        Speed_1MBps,
        Speed_500kBps,
        Speed_250kBps,
        Speed_200kBps,
        Speed_125kBps,
        Speed_100kBps,
        Speed_50kBps,
        Speed_20kBps,
        Count
    };

    static const char* Strings[(size_t)Types::Count];
    static const char* getString(Types type) { return Strings[(int)type]; }
};

struct ECanBus {
    CanTypes::Types type = CanTypes::Types::Disabled;
    CanSpeed::Types speed = CanSpeed::Types::Speed_1MBps;
};
}