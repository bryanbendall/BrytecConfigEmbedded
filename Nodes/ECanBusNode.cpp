#include "ECanBusNode.h"

namespace Brytec {

ENode* ECanBusNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::CanBus || spec.numInputs != 0 || spec.numValues != 5)
        return nullptr;

    return new (destination) ECanBusNodeInternal();
}
}