#include "UsbDefs.h"

#include "Can/EBrytecCan.h"
#include "Deserializer/BinaryArrayDeserializer.h"
#include "Deserializer/BinaryBufferSerializer.h"

namespace Brytec {

UsbPacketType UsbPacket::getType()
{
    return (UsbPacketType)data[0];
}

template <>
void UsbPacket::set(CanExtFrame& frame)
{
    Brytec::BinaryBufferSerializer ser(data, bufferSize);

    ser.writeRaw<uint8_t>((uint8_t)UsbPacketType::Can);
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
CanExtFrame UsbPacket::as()
{
    CanExtFrame frame;

    Brytec::BinaryArrayDeserializer des(data, length);

    UsbPacketType type;
    des.readRaw<uint8_t>((uint8_t*)&type);
    if (type != UsbPacketType::Can)
        return frame;

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

    length = des.getCurrentOffset();

    return frame;
}
}