#include "EToggleNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Toggle>(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Toggle || spec.numInputs != 2 || spec.numValues != 2)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto in = spec.connections[0];
    auto reset = spec.connections[1];

    if (in == Float && reset == Float)
        return new (destination) EToggleNode<float, float>();
    if (in == Float && reset == Pointer)
        return new (destination) EToggleNode<float, float*>();
    if (in == Pointer && reset == Float)
        return new (destination) EToggleNode<float*, float>();
    if (in == Pointer && reset == Pointer)
        return new (destination) EToggleNode<float*, float*>();

#else
    return new (destination) EToggleNode<float, float>();
#endif

    return nullptr;
}

}