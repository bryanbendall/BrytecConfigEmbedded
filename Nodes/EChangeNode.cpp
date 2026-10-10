#include "EChangeNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Change>(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Change || spec.numInputs != 1 || spec.numValues != 3)
        return nullptr;

    ChangeType changeType = (ChangeType)FloatToUint(spec.values[0]);

#if ENODE_FULL_TEMPLATE

    switch (changeType) {
    case ChangeType::Rising:
        if (spec.connections[0] == Float)
            return new (destination) EChangeNode<ChangeType::Rising, float>();
        if (spec.connections[0] == Pointer)
            return new (destination) EChangeNode<ChangeType::Rising, float*>();
    case ChangeType::Falling:
        if (spec.connections[0] == Float)
            return new (destination) EChangeNode<ChangeType::Falling, float>();
        if (spec.connections[0] == Pointer)
            return new (destination) EChangeNode<ChangeType::Falling, float*>();
    case ChangeType::Both:
        if (spec.connections[0] == Float)
            return new (destination) EChangeNode<ChangeType::Both, float>();
        if (spec.connections[0] == Pointer)
            return new (destination) EChangeNode<ChangeType::Both, float*>();
    }

#else
    switch (changeType) {
    case ChangeType::Rising:
        return new (destination) EChangeNode<ChangeType::Rising, float>();
    case ChangeType::Falling:
        return new (destination) EChangeNode<ChangeType::Falling, float>();
    case ChangeType::Both:
        return new (destination) EChangeNode<ChangeType::Both, float>();
    }

#endif

    return nullptr;
}

}