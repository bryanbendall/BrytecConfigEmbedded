#include "UsbBuffer.h"

namespace Brytec {

void UsbBuffer::add(const UsbPacket& packet)
{
    if (m_size >= USB_BUFF_SIZE)
        return;

    m_buffer[m_currentIndex] = packet;
    m_size++;
    m_currentIndex = (m_currentIndex + 1) % USB_BUFF_SIZE;
}

UsbPacket UsbBuffer::get()
{
    if (m_size <= 0)
        return UsbPacket();

    int index = m_currentIndex - m_size;
    if (index < 0)
        index = USB_BUFF_SIZE + index;

    m_size--;
    return m_buffer[index];
}
}