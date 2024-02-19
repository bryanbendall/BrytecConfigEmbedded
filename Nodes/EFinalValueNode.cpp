#include "EFinalValueNode.h"

namespace Brytec {

ENode* EFinalValueNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Final_Value || spec.numInputs != 1 || spec.numValues != 0)
        return nullptr;

#if ENODE_FULL_TEMPLATE
    switch (spec.connections[0]) {
    case Float:
        return new (destination) EFinalValueNodeInternal<float>();
    case Pointer:
        return new (destination) EFinalValueNodeInternal<float*>();
    }

#else
    return new (destination) EFinalValueNodeInternal<float>();

#endif

    return nullptr;
}

}