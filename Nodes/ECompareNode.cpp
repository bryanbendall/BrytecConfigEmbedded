#include "ECompareNode.h"

ENode* ECompareNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Compare || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

    auto compareType = (CompareType)spec.values[0];

#ifdef ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];

    switch (compareType) {
    case CompareType::Equal:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float*, float*>();

    case CompareType::NotEqual:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float*, float*>();

    case CompareType::Greater:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float*, float*>();

    case CompareType::GreaterEqual:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float*, float*>();

    case CompareType::Less:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Less, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Less, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Less, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Less, float*, float*>();

    case CompareType::LessEqual:
        if (input0 == Float && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float*, float*>();

    default:
        break;
    }

#else
    switch (compareType) {
    case CompareType::Equal:
        return new (destination) ECompareNodeInternal<CompareType::Equal, float, float>();

    case CompareType::NotEqual:
        return new (destination) ECompareNodeInternal<CompareType::NotEqual, float, float>();

    case CompareType::Greater:
        return new (destination) ECompareNodeInternal<CompareType::Greater, float, float>();

    case CompareType::GreaterEqual:
        return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float, float>();

    case CompareType::Less:
        return new (destination) ECompareNodeInternal<CompareType::Less, float, float>();

    case CompareType::LessEqual:
        return new (destination) ECompareNodeInternal<CompareType::LessEqual, float, float>();

    default:
        break;
    }

#endif

    return nullptr;
}