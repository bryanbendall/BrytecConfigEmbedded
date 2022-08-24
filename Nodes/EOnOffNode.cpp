#include "EOnOffNode.h"

ENode* EOnOffNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::On_Off || spec.numInputs != 2 || spec.numValues != 0)
        return nullptr;

    auto on = spec.connections[0];
    auto off = spec.connections[1];

#ifdef ENODE_FULL_TEMPLATE
    if (on == Float && off == Float)
        return new (destination) EOnOffNodeInternal<float, float>();
    if (on == Float && off == Pointer)
        return new (destination) EOnOffNodeInternal<float, float*>();
    if (on == Pointer && off == Float)
        return new (destination) EOnOffNodeInternal<float*, float>();
    if (on == Pointer && off == Pointer)
        return new (destination) EOnOffNodeInternal<float*, float*>();

#else
    return new (destination) EOnOffNodeInternal<float, float>();

#endif

    return nullptr;
}
