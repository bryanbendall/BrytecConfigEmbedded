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

struct SerializedNodeInput {
    uint8_t connectionNodeIndex;
    uint8_t outputIndex;
    float defaultValue;

    ConnectionType getConnectionType()
    {
        if (connectionNodeIndex >= 0 && outputIndex >= 0)
            return Pointer;
        else
            return Float;
    }
};

ENode* EMathNode::DeserializeInPlace(BinaryDeserializer& des, uint8_t* destination)
{
    EMathNodeSpecification spec;

    // Inputs
    uint8_t inputCount = des.readRaw<uint8_t>();

    // Input0
    auto in = des.readRaw<SerializedNodeInput>();
    spec.input0 = in.getConnectionType();

    // Input1
    auto in2 = des.readRaw<SerializedNodeInput>();
    spec.input1 = in2.getConnectionType();

    // // Input0
    // if (des.readRaw<int8_t>() >= 0 && des.readRaw<int8_t>() >= 0)
    //     spec.input0 = Pointer;
    // des.readRaw<float>();

    // // Input1
    // if (des.readRaw<int8_t>() >= 0 && des.readRaw<int8_t>() >= 0)
    //     spec.input0 = Pointer;
    // des.readRaw<float>();

    // Values
    uint8_t valueCount = des.readRaw<uint8_t>();

    // Math Type
    spec.type = (MathType)des.readRaw<float>();

    return CreateInPlace(spec, destination);
}
