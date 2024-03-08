#include "EAndNode.h"

namespace Brytec {

ENode* EAndNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::And || spec.numInputs != 5 || spec.numValues != 0)
        return nullptr;

    return new (destination) EAndNodeInternal();
}

}