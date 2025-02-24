#include "EMathNode.h"

namespace Brytec {

#define MathMacroTemplate(type)                                                \
    case type:                                                                 \
        if (input0 == Float && input1 == Float)                                \
            return new (destination) EMathNodeInternal<type, float, float>();  \
        if (input0 == Float && input1 == Pointer)                              \
            return new (destination) EMathNodeInternal<type, float, float*>(); \
        if (input0 == Pointer && input1 == Float)                              \
            return new (destination) EMathNodeInternal<type, float*, float>(); \
        if (input0 == Pointer && input1 == Pointer)                            \
        return new (destination) EMathNodeInternal<type, float*, float*>()

#define MathMacro(type) \
    case type:          \
        return new (destination) EMathNodeInternal<type, float, float>()

ENode* EMathNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Math || spec.numInputs != 2 || spec.numValues != 1)
        return nullptr;

    auto mathType = (MathType)FloatToUint(spec.values[0]);

#if ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];
    auto input1 = spec.connections[1];

    switch (mathType) {

        MathMacroTemplate(MathType::Add);
        MathMacroTemplate(MathType::Subtract);
        MathMacroTemplate(MathType::Multiply);
        MathMacroTemplate(MathType::Divide);
        MathMacroTemplate(MathType::Min);
        MathMacroTemplate(MathType::Max);

    default:
        break;
    }

#else
    switch (mathType) {

        MathMacro(MathType::Add);
        MathMacro(MathType::Subtract);
        MathMacro(MathType::Multiply);
        MathMacro(MathType::Divide);
        MathMacro(MathType::Min);
        MathMacro(MathType::Max);

    default:
        break;
    }

#endif

    return nullptr;
}

}