#include "ENodeDeserializer.h"

namespace Brytec {

ENodeSpec ENodeDeserializer::deserializeNodeSpec(BinaryDeserializer& des)
{
    ENodeSpec spec;

    // Name
    EmptyString empty;
    des.readRaw<EmptyString>(&empty);

    // Type
    uint16_t type;
    des.readRaw<uint16_t>(&type);
    spec.type = (NodeTypes)type;

    // Position
    float x, y;
    des.readRaw<float>(&x);
    des.readRaw<float>(&y);

    // Input connection types (float or pointer)
    {
        des.readRaw<uint8_t>(&spec.numInputs);

        for (int i = 0; i < spec.numInputs; i++) {
            uint8_t connectionType;
            des.readRaw<uint8_t>(&connectionType);
            spec.connections[i] = (ConnectionType)connectionType;
        }
    }

    // Values
    {
        des.readRaw<uint8_t>(&spec.numValues);

        for (int i = 0; i < spec.numValues; i++)
            des.readRaw<float>(&spec.values[i]);
    }

    // Special case for node groups
    if (spec.type == NodeTypes::Node_Group) {
        des.readRaw<uint64_t>(&spec.uuid); // uuid
        // TODO remove module address and pin index from serialization
        uint8_t moduleAddress;
        des.readRaw<uint8_t>(&moduleAddress); // Module Address
        uint16_t pinIndex;
        des.readRaw<uint16_t>(&pinIndex); // Pin Index
    }

    return spec;
}

ENodeConnection ENodeDeserializer::deserializeNodeConnection(BinaryDeserializer& des)
{
    ENodeConnection connection;

    des.readRaw<int16_t>(&connection.connectionNodeIndex);
    des.readRaw<int8_t>(&connection.outputIndex);
    des.readRaw<float>(&connection.defaultValue);

    return connection;
}

}