#include "EPinCurrentNode.h"

namespace Brytec {

ENode* EPinCurrentNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::PinCurrent || spec.numInputs != 0 || spec.numValues != 1)
        return nullptr;

    return new (destination) EPinCurrentNodeInternal();
}

}