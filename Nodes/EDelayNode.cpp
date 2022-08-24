#include "EDelayNode.h"

ENode* EDelayNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Delay || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

    auto in = spec.connections[0];
    auto delayTime = spec.connections[1];

#ifdef ENODE_FULL_TEMPLATE
    if (in == Float && delayTime == Float)
        return new (destination) EDelayNodeInternal<float, float>();
    if (in == Float && delayTime == Pointer)
        return new (destination) EDelayNodeInternal<float, float*>();
    if (in == Pointer && delayTime == Float)
        return new (destination) EDelayNodeInternal<float*, float>();
    if (in == Pointer && delayTime == Pointer)
        return new (destination) EDelayNodeInternal<float*, float*>();

#else
    return new (destination) EDelayNodeInternal<float, float>();

#endif

    return nullptr;
}
