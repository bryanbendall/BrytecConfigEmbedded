#include "EToggleNode.h"

ENode* EToggleNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Toggle || spec.numInputs != 1 || spec.numValues != 2)
        return nullptr;

    auto in = spec.connections[0];

#ifdef ENODE_FULL_TEMPLATE
    if (in == Float)
        return new (destination) EToggleNodeInternal<float>();
    if (in == Pointer)
        return new (destination) EToggleNodeInternal<float*>();

#else
    return new (destination) EToggleNodeInternal<float>();
#endif

    return nullptr;
}