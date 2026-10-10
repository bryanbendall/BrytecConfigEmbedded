#include "EFinalValueNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Final_Value>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Final_Value || spec.numInputs != 1 || spec.numValues != 0)
        return nullptr;

#if ENODE_FULL_TEMPLATE
    switch (spec.connections[0]) {
    case Float:
        return new (destination) EFinalValueNode<float>();
    case Pointer:
        return new (destination) EFinalValueNode<float*>();
    }

#else
    return new (destination) EFinalValueNode<float>();

#endif

    return nullptr;
}

}