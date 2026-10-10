#include "EDeltaTimeNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::DeltaTime>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::DeltaTime || spec.numInputs != 0 || spec.numValues != 0)
        return nullptr;

    return new (destination) EDeltaTimeNode();

    return nullptr;
}

}