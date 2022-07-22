#include "EInvertNode.h"

ENode* EInvertNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Invert || spec.numInputs != 1 || spec.numValues != 0)
        return nullptr;

    auto in = spec.connections[0];

    if (in == Float)
        return new (destination) EInvertNodeInternal<float>();
    if (in == Pointer)
        return new (destination) EInvertNodeInternal<float*>();

    return nullptr;
}
