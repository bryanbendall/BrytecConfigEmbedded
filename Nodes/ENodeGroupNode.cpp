#include "ENodeGroupNode.h"

namespace Brytec {

ENode* ENodeGroupNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Node_Group || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

#ifdef ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];

    if (input0 == Float && input1 == Float)
        return new (destination) ENodeGroupNodeInternal<float, float>(spec.moduleAddress, spec.pinIndex);
    if (input0 == Float && input1 == Pointer)
        return new (destination) ENodeGroupNodeInternal<float, float*>(spec.moduleAddress, spec.pinIndex);
    if (input0 == Pointer && input1 == Float)
        return new (destination) ENodeGroupNodeInternal<float*, float>(spec.moduleAddress, spec.pinIndex);
    if (input0 == Pointer && input1 == Pointer)
        return new (destination) ENodeGroupNodeInternal<float*, float*>(spec.moduleAddress, spec.pinIndex);

#else

    return new (destination) ENodeGroupNodeInternal<float, float>(spec.moduleAddress, spec.pinIndex);

#endif

    return nullptr;
}

}