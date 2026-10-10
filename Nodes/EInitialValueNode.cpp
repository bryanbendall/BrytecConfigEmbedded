#include "EInitialValueNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Initial_Value>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Initial_Value || spec.numInputs != 0 || spec.numValues != 1)
        return nullptr;

    return new (destination) EInitialValueNode();
}

}