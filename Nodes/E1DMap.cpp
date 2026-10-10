#include "E1DMap.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::OneDMap>(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::OneDMap || spec.numInputs != 1 || spec.numValues != (EOneDMapSize * 2))
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto in = spec.connections[0];

    if (in == Float)
        return new (destination) EOneDMapNode<float>();
    if (in == Pointer)
        return new (destination) EOneDMapNode<float*>();

#else
    return new (destination) EOneDMapNode<float>();

#endif

    return nullptr;
}

}