#include "EBrytecCan.h"

#include "Deserializer/BinaryArrayDeserializer.h"
#include "Deserializer/BinaryBufferSerializer.h"
#include <string.h>

namespace Brytec {

bool CanExtFrame::isBroadcast() const
{
    return (id & ((uint32_t)1 << 28));
}

bool CanExtFrame::isPinBroadcast() const
{
    return isBroadcast() && ((id & 0xFFFF) != CanCommands::NoNodeGroup);
}

bool CanExtFrame::isModuleBroadcast() const
{
    return isBroadcast() && ((id & 0xFFFF) == CanCommands::NoNodeGroup);
}

CanCommands::CanCommands(const CanExtFrame& frame)
{
    // Is a broadcast
    if (frame.isBroadcast())
        return;

    command = (Command)((frame.id >> 24) & 0b1111);
    moduleAddress = (frame.id >> 16);
    nodeGroupIndex = frame.id;

    memcpy(data, frame.data, 8);
}

CanExtFrame CanCommands::getFrame()
{
    CanExtFrame frame;

    frame.id = 0;
    // Broadcast Flag 0
    frame.id |= ((uint32_t)(command & 0b1111) << 24);
    frame.id |= ((uint32_t)moduleAddress << 16);
    frame.id |= nodeGroupIndex;

    memcpy(frame.data, data, 8);

    return frame;
}

PinStatusBroadcast::PinStatusBroadcast(const CanExtFrame& frame)
{
    // Not a pin broadcast
    if (!frame.isPinBroadcast())
        return;

    statusFlags = (StatusFlags)((frame.id >> 24) & 0b1111);
    moduleAddress = (frame.id >> 16);
    nodeGroupIndex = frame.id;

    uint16_t tempVoltage = 0;
    uint16_t tempCurrent = 0;
    BinaryArrayDeserializer des(frame.data, 8);
    des.readRaw<uint16_t>(&tempVoltage);
    des.readRaw<uint16_t>(&tempCurrent);
    des.readRaw<float>(&value);
    voltage = (float)tempVoltage / 100.0f;
    current = (float)tempCurrent / 100.0f;
}

CanExtFrame PinStatusBroadcast::getFrame()
{
    CanExtFrame frame;

    frame.id = 0;
    frame.id |= ((uint32_t)1 << 28); // Broadcast Flag
    frame.id |= ((uint32_t)(statusFlags & 0b1111) << 24);
    frame.id |= ((uint32_t)moduleAddress << 16);
    frame.id |= nodeGroupIndex;

    BinaryBufferSerializer ser(frame.data, 8);
    ser.writeRaw<uint16_t>((uint16_t)(voltage * 100.0f));
    ser.writeRaw<uint16_t>((uint16_t)(current * 100.0f));
    ser.writeRaw<float>(value);

    return frame;
}

ModuleStatusBroadcast::ModuleStatusBroadcast(const CanExtFrame& frame)
{
    // Not a module broadcast
    if (!frame.isModuleBroadcast())
        return;

    moduleAddress = (frame.id >> 16);

    BinaryArrayDeserializer des(frame.data, 8);
    des.readRaw<uint8_t>(&mode);
    des.readRaw<bool>(&deserializeOk);
    des.readRaw<uint16_t>(&nodeArraySize);
}

CanExtFrame ModuleStatusBroadcast::getFrame()
{
    CanExtFrame frame;

    frame.id = 0;
    frame.id |= ((uint32_t)1 << 28); // Broadcast Flag
    frame.id |= ((uint32_t)moduleAddress << 16);
    frame.id |= nodeGroupIndex;

    BinaryBufferSerializer ser(frame.data, 8);
    ser.writeRaw<uint8_t>((uint8_t)mode);
    ser.writeRaw<bool>(deserializeOk);
    ser.writeRaw<uint16_t>(nodeArraySize);

    return frame;
}
}