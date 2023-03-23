#include "EPIDNode.h"

namespace Brytec {

ENode* EPIDNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::PID || spec.numInputs != 5 || spec.numValues != 2)
        return nullptr;

#ifdef ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];
    auto input2 = spec.connections[2];
    auto input3 = spec.connections[3];
    auto input4 = spec.connections[4];

    if (input0 == Float && input1 == Float && input2 == Float && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float, float, float, float>();
    if (input0 == Float && input1 == Float && input2 == Float && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float, float, float, float*>();

    if (input0 == Float && input1 == Float && input2 == Float && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float, float, float*, float>();
    if (input0 == Float && input1 == Float && input2 == Float && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float, float, float*, float*>();

    if (input0 == Float && input1 == Float && input2 == Pointer && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float, float*, float, float>();
    if (input0 == Float && input1 == Float && input2 == Pointer && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float, float*, float, float*>();

    if (input0 == Float && input1 == Float && input2 == Pointer && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float, float*, float*, float>();
    if (input0 == Float && input1 == Float && input2 == Pointer && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float, float*, float*, float*>();

    if (input0 == Float && input1 == Pointer && input2 == Float && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float*, float, float, float>();
    if (input0 == Float && input1 == Pointer && input2 == Float && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float*, float, float, float*>();

    if (input0 == Float && input1 == Pointer && input2 == Float && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float*, float, float*, float>();
    if (input0 == Float && input1 == Pointer && input2 == Float && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float*, float, float*, float*>();

    if (input0 == Float && input1 == Pointer && input2 == Pointer && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float*, float*, float, float>();
    if (input0 == Float && input1 == Pointer && input2 == Pointer && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float*, float*, float, float*>();

    if (input0 == Float && input1 == Pointer && input2 == Pointer && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float, float*, float*, float*, float>();
    if (input0 == Float && input1 == Pointer && input2 == Pointer && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float, float*, float*, float*, float*>();

    if (input0 == Pointer && input1 == Float && input2 == Float && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float, float, float, float>();
    if (input0 == Pointer && input1 == Float && input2 == Float && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float, float, float, float*>();

    if (input0 == Pointer && input1 == Float && input2 == Float && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float, float, float*, float>();
    if (input0 == Pointer && input1 == Float && input2 == Float && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float, float, float*, float*>();

    if (input0 == Pointer && input1 == Float && input2 == Pointer && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float, float*, float, float>();
    if (input0 == Pointer && input1 == Float && input2 == Pointer && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float, float*, float, float*>();

    if (input0 == Pointer && input1 == Float && input2 == Pointer && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float, float*, float*, float>();
    if (input0 == Pointer && input1 == Float && input2 == Pointer && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float, float*, float*, float*>();

    if (input0 == Pointer && input1 == Pointer && input2 == Float && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float*, float, float, float>();
    if (input0 == Pointer && input1 == Pointer && input2 == Float && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float*, float, float, float*>();

    if (input0 == Pointer && input1 == Pointer && input2 == Float && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float*, float, float*, float>();
    if (input0 == Pointer && input1 == Pointer && input2 == Float && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float*, float, float*, float*>();

    if (input0 == Pointer && input1 == Pointer && input2 == Pointer && input3 == Float && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float*, float*, float, float>();
    if (input0 == Pointer && input1 == Pointer && input2 == Pointer && input3 == Float && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float*, float*, float, float*>();

    if (input0 == Pointer && input1 == Pointer && input2 == Pointer && input3 == Pointer && input4 == Float)
        return new (destination) EPIDNodeInternal<float*, float*, float*, float*, float>();
    if (input0 == Pointer && input1 == Pointer && input2 == Pointer && input3 == Pointer && input4 == Pointer)
        return new (destination) EPIDNodeInternal<float*, float*, float*, float*, float*>();

#else
    return new (destination) EPIDNodeInternal<float, float, float, float, float>();
#endif

    return nullptr;
}

}