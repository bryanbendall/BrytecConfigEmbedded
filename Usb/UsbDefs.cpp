#include "UsbDefs.h"

#include "Can/EBrytecCan.h"
#include "Deserializer/BinaryArrayDeserializer.h"
#include "Deserializer/BinaryBufferSerializer.h"
#include "ModuleCommand.h"

namespace Brytec {

UsbPacketType UsbPacket::getType()
{
    return (UsbPacketType)data[0];
}

template <>
void UsbPacket::set(const CanFrame& frame)
{
    Brytec::BinaryBufferSerializer ser(data, bufferSize);

    ser.writeRaw<uint8_t>((uint8_t)UsbPacketType::Can);
    ser.writeRaw<uint8_t>((uint8_t)frame.type);
    ser.writeRaw<uint32_t>(frame.id);
    ser.writeRaw<uint8_t>(frame.dlc);
    ser.writeRaw<uint8_t>(frame.data[0]);
    ser.writeRaw<uint8_t>(frame.data[1]);
    ser.writeRaw<uint8_t>(frame.data[2]);
    ser.writeRaw<uint8_t>(frame.data[3]);
    ser.writeRaw<uint8_t>(frame.data[4]);
    ser.writeRaw<uint8_t>(frame.data[5]);
    ser.writeRaw<uint8_t>(frame.data[6]);
    ser.writeRaw<uint8_t>(frame.data[7]);

    length = ser.getWroteLenth();
}

template <>
void UsbPacket::set(const ModuleCommand& moduleCommand)
{
    Brytec::BinaryBufferSerializer ser(data, bufferSize);

    ser.writeRaw<uint8_t>((uint8_t)UsbPacketType::Command);
    ser.writeRaw<uint8_t>((uint8_t)moduleCommand.command);

    length = ser.getWroteLenth();
}

template <>
CanFrame UsbPacket::as() const
{
    CanFrame frame;

    Brytec::BinaryArrayDeserializer des(data, length);

    UsbPacketType type;
    des.readRaw<uint8_t>((uint8_t*)&type);
    if (type != UsbPacketType::Can)
        return frame;

    des.readRaw<uint8_t>((uint8_t*)&frame.type);
    des.readRaw<uint32_t>(&frame.id);
    des.readRaw<uint8_t>(&frame.dlc);
    des.readRaw<uint8_t>(&frame.data[0]);
    des.readRaw<uint8_t>(&frame.data[1]);
    des.readRaw<uint8_t>(&frame.data[2]);
    des.readRaw<uint8_t>(&frame.data[3]);
    des.readRaw<uint8_t>(&frame.data[4]);
    des.readRaw<uint8_t>(&frame.data[5]);
    des.readRaw<uint8_t>(&frame.data[6]);
    des.readRaw<uint8_t>(&frame.data[7]);

    return frame;
}

template <>
ModuleCommand UsbPacket::as() const
{
    ModuleCommand moduleCommand;

    Brytec::BinaryArrayDeserializer des(data, length);

    UsbPacketType type;
    des.readRaw<uint8_t>((uint8_t*)&type);
    if (type != UsbPacketType::Command)
        return moduleCommand;

    des.readRaw<uint8_t>((uint8_t*)&moduleCommand.command);

    return moduleCommand;
}
}