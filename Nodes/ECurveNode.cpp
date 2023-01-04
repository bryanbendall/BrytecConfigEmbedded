#include "ECurveNode.h"

ENode* ECurveNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Curve || spec.numInputs != 3 || spec.numValues != 2)
        return nullptr;

    auto in = spec.connections[0];
    auto repeat = spec.connections[1];
    auto timeout = spec.connections[2];
    auto curveType = (CurveType)spec.values[0];

#ifdef ENODE_FULL_TEMPLATE
    switch (curveType) {
    case CurveType::Toggle:
        if (in == Float && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float, float, float>();
        if (in == Float && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float, float, float*>();

        if (in == Float && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float, float*, float>();
        if (in == Float && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float, float*, float*>();

        if (in == Pointer && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float*, float, float>();
        if (in == Pointer && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float*, float, float*>();

        if (in == Pointer && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float*, float*, float>();
        if (in == Pointer && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Toggle, float*, float*, float*>();

    case CurveType::Linear:
        if (in == Float && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float, float, float>();
        if (in == Float && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float, float, float*>();

        if (in == Float && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float, float*, float>();
        if (in == Float && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float, float*, float*>();

        if (in == Pointer && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float*, float, float>();
        if (in == Pointer && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float*, float, float*>();

        if (in == Pointer && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float*, float*, float>();
        if (in == Pointer && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Linear, float*, float*, float*>();

    case CurveType::Exponential:
        if (in == Float && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float, float, float>();
        if (in == Float && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float, float, float*>();

        if (in == Float && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float, float*, float>();
        if (in == Float && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float, float*, float*>();

        if (in == Pointer && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float*, float, float>();
        if (in == Pointer && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float*, float, float*>();

        if (in == Pointer && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float*, float*, float>();
        if (in == Pointer && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Exponential, float*, float*, float*>();

    case CurveType::Breathing:
        if (in == Float && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float, float, float>();
        if (in == Float && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float, float, float*>();

        if (in == Float && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float, float*, float>();
        if (in == Float && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float, float*, float*>();

        if (in == Pointer && repeat == Float && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float*, float, float>();
        if (in == Pointer && repeat == Float && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float*, float, float*>();

        if (in == Pointer && repeat == Pointer && timeout == Float)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float*, float*, float>();
        if (in == Pointer && repeat == Pointer && timeout == Pointer)
            return new (destination) ECurveNodeInternal<CurveType::Breathing, float*, float*, float*>();

    default:
        break;
    }

#else
    switch (curveType) {
    case CurveType::Toggle:
        return new (destination) ECurveNodeInternal<CurveType::Toggle, float, float, float>();

    case CurveType::Linear:
        return new (destination) ECurveNodeInternal<CurveType::Linear, float, float, float>();

    case CurveType::Exponential:
        return new (destination) ECurveNodeInternal<CurveType::Exponential, float, float, float>();

    case CurveType::Breathing:
        return new (destination) ECurveNodeInternal<CurveType::Breathing, float, float, float>();

    default:
        break;
    }

#endif

    return nullptr;
}
