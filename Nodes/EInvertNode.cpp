#include "EInvertNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Invert>(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Invert || spec.numInputs != 1 || spec.numValues != 0)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto in = spec.connections[0];

    if (in == Float)
        return new (destination) EInvertNode<float>();
    if (in == Pointer)
        return new (destination) EInvertNode<float*>();

#else
    return new (destination) EInvertNode<float>();

#endif

    return nullptr;
}

}