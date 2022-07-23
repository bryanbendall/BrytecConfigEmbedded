#include "ENodeGroupNode.h"

ENode* ENodeGroupNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Node_Group || spec.numInputs != 0 || spec.numValues != 1)
        return nullptr;

    return new (destination) ENodeGroupNodeInternal();
}