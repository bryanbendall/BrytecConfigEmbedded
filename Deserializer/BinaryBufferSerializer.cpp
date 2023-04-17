#include "BinaryBufferSerializer.h"

namespace Brytec {

BinaryBufferSerializer::BinaryBufferSerializer(uint8_t* buffer, uint32_t bufferLength)
    : m_buffer(buffer)
    , m_bufferLength(bufferLength)
{
}

}