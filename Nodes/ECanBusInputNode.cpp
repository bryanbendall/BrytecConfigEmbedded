#include "ECanBusInputNode.h"

namespace Brytec {

ENode* ECanBusInputNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::CanBusInput || spec.numInputs != 0 || spec.numValues != 5)
        return nullptr;

    return new (destination) ECanBusInputNodeInternal();
}
}