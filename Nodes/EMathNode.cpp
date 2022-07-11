#include "EMathNode.h"
#include "BrytecConfigEmbedded/Utils/ENodeDeserializer.h"
#include <new>

ENode* EMathNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Math || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

    auto input0 = spec.connections[0].type;
    auto input1 = spec.connections[1].type;
    auto mathType = (MathType)spec.values[0];

    switch (mathType) {
    case MathType::Add:
        if (input0 == Float && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Add, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Add, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Add, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Add, float*, float*>();

    case MathType::Subtract:
        if (input0 == Float && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Subtract, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Subtract, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Subtract, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Subtract, float*, float*>();

    case MathType::Multiply:
        if (input0 == Float && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Multiply, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Multiply, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Multiply, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Multiply, float*, float*>();

    case MathType::Divide:
        if (input0 == Float && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Divide, float, float>();
        if (input0 == Float && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Divide, float, float*>();
        if (input0 == Pointer && input1 == Float)
            return new (destination) EMathNodeInternal<MathType::Divide, float*, float>();
        if (input0 == Pointer && input1 == Pointer)
            return new (destination) EMathNodeInternal<MathType::Divide, float*, float*>();
    }

    return nullptr;
}