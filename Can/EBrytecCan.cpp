#include "EBrytecCan.h"

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

    uint64_t* data = (uint64_t*)frame.data;
    uint16_t tempCurrent = ((*data) >> 48);
    current = (float)tempCurrent / 100.0f;
    uint16_t tempVoltage = (uint16_t)((*data) >> 32);
    voltage = (float)tempVoltage / 100.0f;
    value = *((float*)data);
}

CanExtFrame PinStatusBroadcast::getFrame()
{
    CanExtFrame frame;

    frame.id = 0;
    frame.id |= ((uint32_t)1 << 28); // Broadcast Flag
    frame.id |= ((uint32_t)(statusFlags & 0b1111) << 24);
    frame.id |= ((uint32_t)moduleAddress << 16);
    frame.id |= nodeGroupIndex;

    uint64_t* data = (uint64_t*)frame.data;
    *data = 0;

    uint16_t tempCurrent = (uint16_t)(current * 100.0f);
    *data |= (uint64_t)tempCurrent << 48;

    uint16_t tempVoltage = (uint16_t)(voltage * 100.0f);
    *data |= (uint64_t)tempVoltage << 32;

    uint32_t* tempValue = (uint32_t*)&value;
    *data |= *tempValue;

    return frame;
}

ModuleStatusBroadcast::ModuleStatusBroadcast(const CanExtFrame& frame)
{
    // Not a module broadcast
    if (!frame.isModuleBroadcast())
        return;

    moduleAddress = (frame.id >> 16);
    nodeArraySize = frame.id;

    mode = frame.data[0];
    deserializeOk = frame.data[1];
}

CanExtFrame ModuleStatusBroadcast::getFrame()
{
    CanExtFrame frame;

    frame.id = 0;
    frame.id |= ((uint32_t)1 << 28); // Broadcast Flag
    frame.id |= ((uint32_t)moduleAddress << 16);
    frame.id |= nodeArraySize;

    frame.data[0] = (uint8_t)mode;
    frame.data[1] = deserializeOk;

    return frame;
}
}