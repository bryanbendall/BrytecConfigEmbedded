#include "ECanBusOutputNode.h"

namespace Brytec {

ENode* ECanBusOutputNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::CanBusOutput || spec.numInputs != 3 || spec.numValues != 6)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];
    auto input2 = spec.connections[2];

    if (input0 == Float && input1 == Float && input2 == Float)
        return new (destination) ECanBusOutputNodeInternal<float, float, float>();
    if (input0 == Float && input1 == Float && input2 == Pointer)
        return new (destination) ECanBusOutputNodeInternal<float, float, float*>();

    if (input0 == Float && input1 == Pointer && input2 == Float)
        return new (destination) ECanBusOutputNodeInternal<float, float*, float>();
    if (input0 == Float && input1 == Pointer && input2 == Pointer)
        return new (destination) ECanBusOutputNodeInternal<float, float*, float*>();

    if (input0 == Pointer && input1 == Float && input2 == Float)
        return new (destination) ECanBusOutputNodeInternal<float*, float, float>();
    if (input0 == Pointer && input1 == Float && input2 == Pointer)
        return new (destination) ECanBusOutputNodeInternal<float*, float, float*>();

    if (input0 == Pointer && input1 == Pointer && input2 == Float)
        return new (destination) ECanBusOutputNodeInternal<float*, float*, float>();
    if (input0 == Pointer && input1 == Pointer && input2 == Pointer)
        return new (destination) ECanBusOutputNodeInternal<float*, float*, float*>();

#else
    return new (destination) ECanBusOutputNodeInternal<float, float, float>();
#endif

    return nullptr;
}

template <>
void setData<float>(const float& value, uint8_t* dest, ECanBusOutputNode::Endian endian)
{
    uint8_t typeSize = sizeof(float);
    uint8_t* tempValue = (uint8_t*)&value;
    for (uint8_t index = 0; index < typeSize; index++) {
        if (endian == ECanBusOutputNode::Endian::Little)
            dest[index] = tempValue[index];
        if (endian == ECanBusOutputNode::Endian::Big)
            dest[typeSize - 1 - index] = tempValue[index];
    }
}
}