#pragma once

#include <stdint.h>

namespace Brytec {

struct ModuleCommand {
    enum Command : uint8_t {
        None,
        GoToBootloader
    };

    Command command = Command::None;
    explicit operator bool() const { return command != Command::None; }
};

}