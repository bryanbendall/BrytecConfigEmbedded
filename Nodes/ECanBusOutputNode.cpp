#include "ECanBusOutputNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::CanBusOutput>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::CanBusOutput || spec.numInputs != 2 || spec.numValues != 7)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];

    if (input0 == Float && input1 == Float)
        return new (destination) ECanBusOutputNode<float, float>();
    if (input0 == Float && input1 == Pointer)
        return new (destination) ECanBusOutputNode<float, float*>();

    if (input0 == Pointer && input1 == Float)
        return new (destination) ECanBusOutputNode<float*, float>();
    if (input0 == Pointer && input1 == Pointer)
        return new (destination) ECanBusOutputNode<float*, float*>();

#else
    return new (destination) ECanBusOutputNode<float, float>();
#endif

    return nullptr;
}

template <>
void setData<float>(const float& value, uint8_t* dest, CanEndian endian)
{
    uint8_t typeSize = sizeof(float);
    uint8_t* tempValue = (uint8_t*)&value;
    for (uint8_t index = 0; index < typeSize; index++) {
        if (endian == CanEndian::Little)
            dest[index] = tempValue[index];
        if (endian == CanEndian::Big)
            dest[typeSize - 1 - index] = tempValue[index];
    }
}
}