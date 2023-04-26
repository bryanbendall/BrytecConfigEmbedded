#pragma once

#include "UsbDefs.h"

#define USB_BUFF_SIZE 10

namespace Brytec {

class UsbBuffer {

public:
    void add(const UsbPacket& packet);
    UsbPacket get();

    uint32_t size() { return m_size; }

private:
    UsbPacket m_buffer[USB_BUFF_SIZE];
    uint32_t m_currentIndex = 0;
    uint32_t m_size = 0;
};
}