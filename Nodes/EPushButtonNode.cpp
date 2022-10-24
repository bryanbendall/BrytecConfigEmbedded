#include "EPushButtonNode.h"

ENode* EPushButtonNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Push_Button || spec.numInputs != 3 || spec.numValues != 3)
        return nullptr;

    auto button = spec.connections[0];
    auto neutralSafety = spec.connections[1];
    auto engineRunning = spec.connections[2];

#ifdef ENODE_FULL_TEMPLATE
    if (button == Float && neutralSafety == Float && engineRunning == Float)
        return new (destination) EPushButtonNodeInternal<float, float, float>();
    if (button == Float && neutralSafety == Float && engineRunning == Pointer)
        return new (destination) EPushButtonNodeInternal<float, float, float*>();

    if (button == Float && neutralSafety == Pointer && engineRunning == Float)
        return new (destination) EPushButtonNodeInternal<float, float*, float>();
    if (button == Float && neutralSafety == Pointer && engineRunning == Pointer)
        return new (destination) EPushButtonNodeInternal<float, float*, float*>();

    if (button == Pointer && neutralSafety == Float && engineRunning == Float)
        return new (destination) EPushButtonNodeInternal<float*, float, float>();
    if (button == Pointer && neutralSafety == Float && engineRunning == Pointer)
        return new (destination) EPushButtonNodeInternal<float*, float, float*>();

    if (button == Pointer && neutralSafety == Pointer && engineRunning == Float)
        return new (destination) EPushButtonNodeInternal<float*, float*, float>();
    if (button == Pointer && neutralSafety == Pointer && engineRunning == Pointer)
        return new (destination) EPushButtonNodeInternal<float*, float*, float*>();

#else
    return new (destination) EPushButtonNodeInternal<float, float, float>();

#endif

    return nullptr;
}
