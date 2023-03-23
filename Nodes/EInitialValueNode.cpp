#include "EInitialValueNode.h"

namespace Brytec {

ENode* EInitialValueNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Initial_Value || spec.numInputs != 0 || spec.numValues != 1)
        return nullptr;

    return new (destination) EInitialValueNodeInternal();
}

}