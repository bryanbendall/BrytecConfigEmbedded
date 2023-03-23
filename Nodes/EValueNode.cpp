#include "EValueNode.h"

namespace Brytec {

ENode* EValueNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Value || spec.numInputs != 0 || spec.numValues != 1)
        return nullptr;

    return new (destination) EValueNodeInternal();
}

}