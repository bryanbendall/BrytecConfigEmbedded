#include "ECounterNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Counter>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Counter || spec.numInputs != 5 || spec.numValues != 3)
        return nullptr;

    return new (destination) ECounterNode();

    return nullptr;
}

}