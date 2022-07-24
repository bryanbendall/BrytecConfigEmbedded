#include "ENodeDeserializer.h"

ENodeSpec ENodeDeserializer::deserializeNodeSpec(BinaryDeserializer& des)
{
    ENodeSpec spec;

    des.readRaw<std::string>(); // name
    spec.type = (NodeTypes)des.readRaw<uint16_t>();
    des.readRaw<float>(); // x
    des.readRaw<float>(); // y

    // Input types
    {
        spec.numInputs = des.readRaw<uint8_t>();

        for (int i = 0; i < spec.numInputs; i++) {
            spec.connections[i] = (ConnectionType)des.readRaw<uint8_t>();
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

ENodeConnection ENodeDeserializer::deserializeNodeConnection(BinaryDeserializer& des)
{
    ENodeConnection connection;

    connection.connectionNodeIndex = des.readRaw<int8_t>();
    connection.outputIndex = des.readRaw<int8_t>();
    connection.defaultValue = des.readRaw<float>();

    return connection;
}