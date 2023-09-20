#pragma once

#include "EBrytecConfig.h"
#include "UsbDefs.h"

namespace Brytec {

class UsbBuffer {

public:
    void add(const UsbPacket& packet);
    UsbPacket get();

    uint32_t size() { return m_size; }

private:
    UsbPacket m_buffer[USB_BUFFER_SIZE];
    uint32_t m_currentIndex = 0;
    uint32_t m_size = 0;
};
}