#include "EPreviousValueNode.h"

namespace Brytec {

ENode* EPreviousValueNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::PreviousValue || spec.numInputs != 0 || spec.numValues != 0)
        return nullptr;

    return new (destination) EPreviousValueNodeInternal();

    return nullptr;
}

}