#include "EMixColorNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Mix_Color>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Mix_Color || spec.numInputs != 3 || spec.numValues != 0)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];
    auto input2 = spec.connections[2];

    if (input0 == Float && input1 == Float && input2 == Float)
        return new (destination) EMixColorNode<float, float, float>();
    if (input0 == Float && input1 == Float && input2 == Pointer)
        return new (destination) EMixColorNode<float, float, float*>();

    if (input0 == Float && input1 == Pointer && input2 == Float)
        return new (destination) EMixColorNode<float, float*, float>();
    if (input0 == Float && input1 == Pointer && input2 == Pointer)
        return new (destination) EMixColorNode<float, float*, float*>();

    if (input0 == Pointer && input1 == Float && input2 == Float)
        return new (destination) EMixColorNode<float*, float, float>();
    if (input0 == Pointer && input1 == Float && input2 == Pointer)
        return new (destination) EMixColorNode<float*, float, float*>();

    if (input0 == Pointer && input1 == Pointer && input2 == Float)
        return new (destination) EMixColorNode<float*, float*, float>();
    if (input0 == Pointer && input1 == Pointer && input2 == Pointer)
        return new (destination) EMixColorNode<float*, float*, float*>();

#else
    return new (destination) EMixColorNode<float, float, float>();
#endif

    return nullptr;
}

}