#include "ESwitchNode.h"

ENode* ESwitchNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Switch || spec.numInputs != 3 || spec.numValues != 0)
        return nullptr;

    auto selection = spec.connections[0];
    auto input1 = spec.connections[1];
    auto input2 = spec.connections[2];

    if (selection == Float && input1 == Float && input2 == Float)
        return new (destination) ESwitchNodeInternal<float, float, float>();
    if (selection == Float && input1 == Float && input2 == Pointer)
        return new (destination) ESwitchNodeInternal<float, float, float*>();

    if (selection == Float && input1 == Pointer && input2 == Float)
        return new (destination) ESwitchNodeInternal<float, float*, float>();
    if (selection == Float && input1 == Pointer && input2 == Pointer)
        return new (destination) ESwitchNodeInternal<float, float*, float*>();

    if (selection == Pointer && input1 == Float && input2 == Float)
        return new (destination) ESwitchNodeInternal<float*, float, float>();
    if (selection == Pointer && input1 == Float && input2 == Pointer)
        return new (destination) ESwitchNodeInternal<float*, float, float*>();

    if (selection == Pointer && input1 == Pointer && input2 == Float)
        return new (destination) ESwitchNodeInternal<float*, float*, float>();
    if (selection == Pointer && input1 == Pointer && input2 == Pointer)
        return new (destination) ESwitchNodeInternal<float*, float*, float*>();

    return nullptr;
}
