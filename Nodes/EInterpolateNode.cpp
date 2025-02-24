#include "EInterpolateNode.h"

namespace Brytec {

#define InterpolateMacroTemplate(type)                                                \
    case type:                                                                        \
        if (in == Float && transitionTime == Float)                                   \
            return new (destination) EInterpolateNodeInternal<type, float, float>();  \
        if (in == Float && transitionTime == Pointer)                                 \
            return new (destination) EInterpolateNodeInternal<type, float, float*>(); \
        if (in == Pointer && transitionTime == Float)                                 \
            return new (destination) EInterpolateNodeInternal<type, float*, float>(); \
        if (in == Pointer && transitionTime == Pointer)                               \
        return new (destination) EInterpolateNodeInternal<type, float*, float*>()

#define InterpolateMacro(type) \
    case type:                 \
        return new (destination) EInterpolateNodeInternal<type, float, float>()

ENode* EInterpolateNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Interpolate || spec.numInputs != 2 || spec.numValues != 3)
        return nullptr;

    auto interpolateType = (InterpolateType)FloatToUint(spec.values[0]);

#if ENODE_FULL_TEMPLATE

    auto in = spec.connections[0];
    auto transitionTime = spec.connections[1];

    switch (interpolateType) {

        InterpolateMacroTemplate(InterpolateType::Linear);
        InterpolateMacroTemplate(InterpolateType::EaseInExpo);
        InterpolateMacroTemplate(InterpolateType::EaseOutExpo);
        InterpolateMacroTemplate(InterpolateType::EaseInOutExpo);
        InterpolateMacroTemplate(InterpolateType::EaseInCubic);
        InterpolateMacroTemplate(InterpolateType::EaseOutCubic);
        InterpolateMacroTemplate(InterpolateType::EaseInOutCubic);

    default:
        break;
    }

#else
    switch (interpolateType) {
        InterpolateMacro(InterpolateType::Linear);
        InterpolateMacro(InterpolateType::EaseInExpo);
        InterpolateMacro(InterpolateType::EaseOutExpo);
        InterpolateMacro(InterpolateType::EaseInOutExpo);
        InterpolateMacro(InterpolateType::EaseInCubic);
        InterpolateMacro(InterpolateType::EaseOutCubic);
        InterpolateMacro(InterpolateType::EaseInOutCubic);

    default:
        break;
    }

#endif

    return nullptr;
}

}