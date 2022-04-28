#include "EMathNode.h"
#include <new>

ENode* EMathNode::CreateInPlace(EMathNodeSpecification spec, uint8_t* destination)
{
    switch (spec.type) {
    case MathType::Add:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Add, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Add, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Add, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Add, float*, float*>();

    case MathType::Subtract:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Subtract, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Subtract, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Subtract, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Subtract, float*, float*>();

    case MathType::Multiply:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Multiply, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Multiply, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Multiply, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Multiply, float*, float*>();

    case MathType::Divide:
        if (spec.input0 == Float && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Divide, float, float>();
        if (spec.input0 == Float && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Divide, float, float*>();
        if (spec.input0 == Pointer && spec.input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Divide, float*, float>();
        if (spec.input0 == Pointer && spec.input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Divide, float*, float*>();
    }

    return nullptr;
}

ENode* EMathNode::DeserializeInPlace(BinaryDeserializer& des, uint8_t* destination)
{
    EMathNodeSpecification spec;
    spec.type = (MathType)des.readRaw<int>();
    spec.input0 = (ConnectionType)des.readRaw<int>();
    spec.input1 = (ConnectionType)des.readRaw<int>();
    return CreateInPlace(spec, destination);
}
