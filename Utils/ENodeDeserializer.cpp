#include "ENodeDeserializer.h"

ENodeSpec ENodeDeserializer::deserializeNode(BinaryDeserializer& des)
{
    ENodeSpec spec;

    des.readRaw<std::string>(); // name
    spec.type = (NodeTypes)des.readRaw<uint16_t>();
    des.readRaw<float>(); // x
    des.readRaw<float>(); // y

    // Connections
    {
        spec.numInputs = des.readRaw<uint8_t>();

        for (int i = 0; i < spec.numInputs; i++) {
            spec.connections[i].connectionNodeIndex = des.readRaw<uint8_t>();
            spec.connections[i].outputIndex = des.readRaw<uint8_t>();
            spec.connections[i].defaultValue = des.readRaw<float>();

            if (spec.connections[i].connectionNodeIndex > -1 && spec.connections[i].outputIndex > -1)
                spec.connections[i].type = ConnectionType::Pointer;
        }
    }

    // Values
    {
        spec.numValues = des.readRaw<uint8_t>();

        for (int i = 0; i < spec.numValues; i++)
            spec.values[i] = des.readRaw<float>();
    }

    if (spec.type == NodeTypes::Node_Group)
        des.readRaw<uint64_t>(); // uuid

    return spec;
}