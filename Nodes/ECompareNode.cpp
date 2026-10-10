#include "ECompareNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Compare>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Compare || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

    auto compareType = (CompareType)FloatToUint(spec.values[0]);

#if ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];

    switch (compareType) {
    case CompareType::Equal:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNode<CompareType::Equal, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::Equal, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNode<CompareType::Equal, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::Equal, float*, float*>();

    case CompareType::NotEqual:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNode<CompareType::NotEqual, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::NotEqual, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNode<CompareType::NotEqual, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::NotEqual, float*, float*>();

    case CompareType::Greater:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNode<CompareType::Greater, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::Greater, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNode<CompareType::Greater, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::Greater, float*, float*>();

    case CompareType::GreaterEqual:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNode<CompareType::GreaterEqual, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::GreaterEqual, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNode<CompareType::GreaterEqual, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::GreaterEqual, float*, float*>();

    case CompareType::Less:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNode<CompareType::Less, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::Less, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNode<CompareType::Less, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::Less, float*, float*>();

    case CompareType::LessEqual:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNode<CompareType::LessEqual, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::LessEqual, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNode<CompareType::LessEqual, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNode<CompareType::LessEqual, float*, float*>();

    default:
        break;
    }

#else
    switch (compareType) {
    case CompareType::Equal:
        return new (destination) ECompareNode<CompareType::Equal, float, float>();

    case CompareType::NotEqual:
        return new (destination) ECompareNode<CompareType::NotEqual, float, float>();

    case CompareType::Greater:
        return new (destination) ECompareNode<CompareType::Greater, float, float>();

    case CompareType::GreaterEqual:
        return new (destination) ECompareNode<CompareType::GreaterEqual, float, float>();

    case CompareType::Less:
        return new (destination) ECompareNode<CompareType::Less, float, float>();

    case CompareType::LessEqual:
        return new (destination) ECompareNode<CompareType::LessEqual, float, float>();

    default:
        break;
    }

#endif

    return nullptr;
}

}