#include "ECanBusInputNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::CanBusInput>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::CanBusInput || spec.numInputs != 0 || spec.numValues != 5)
        return nullptr;

    return new (destination) ECanBusInputNode();
}
}