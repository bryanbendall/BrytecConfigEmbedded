#include "EDelayNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Delay>(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Delay || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto in = spec.connections[0];
    auto delayTime = spec.connections[1];

    if (in == Float && delayTime == Float)
        return new (destination) EDelayNode<float, float>();
    if (in == Float && delayTime == Pointer)
        return new (destination) EDelayNode<float, float*>();
    if (in == Pointer && delayTime == Float)
        return new (destination) EDelayNode<float*, float>();
    if (in == Pointer && delayTime == Pointer)
        return new (destination) EDelayNode<float*, float*>();

#else
    return new (destination) EDelayNode<float, float>();

#endif

    return nullptr;
}

}