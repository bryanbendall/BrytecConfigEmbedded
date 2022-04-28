#include "ECompareNode.h"

#include <new>

ENode* ECompareNode::CreateInPlace(ECompareNodeSpecification spec, uint8_t* destination)
{
    switch (spec.type) {
    case CompareType::Equal:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Equal, float*, float*>();

    case CompareType::NotEqual:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::NotEqual, float*, float*>();

    case CompareType::Greater:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Greater, float*, float*>();

    case CompareType::GreaterEqual:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::GreaterEqual, float*, float*>();

    case CompareType::Less:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Less, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Less, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::Less, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::Less, float*, float*>();

    case CompareType::LessEqual:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) ECompareNodeInternal<CompareType::LessEqual, float*, float*>();
    }

    return nullptr;
}

ENode* ECompareNode::DeserializeInPlace(BinaryDeserializer& des, uint8_t* destination)
{
    ECompareNodeSpecification spec;
    spec.type = (CompareType)des.readRaw<int>();
    spec.input0 = (ConnectionType)des.readRaw<int>();
    spec.input1 = (ConnectionType)des.readRaw<int>();
    return CreateInPlace(spec, destination);
}
