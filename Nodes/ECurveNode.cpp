#include "ECurveNode.h"

ENode* ECurveNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Curve || spec.numInputs != 3 || spec.numValues != 2)
        return nullptr;

    auto in = spec.connections[0];
    auto repeat = spec.connections[1];
    auto timeout = spec.connections[2];
    auto curveType = (CurveType)spec.values[0];

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
    }

    return nullptr;
}

    // bool in = ToBool(m_mask.isPointer(0) ? *m_in.pointer : m_in.value);
    // bool repeat = ToBool(m_mask.isPointer(1) ? *m_repeat.pointer : m_repeat.value);
    // float& timeout = m_mask.isPointer(2) ? *m_timeout.pointer : m_timeout.value;
    // Types curveType = (Types)(m_mask.isPointer(3) ? *m_curveType.pointer : m_curveType.value);
    // float& timerCounter = m_mask.isPointer(4) ? *m_timerCounter.pointer : m_timerCounter.value;

    // if (!in) {
    //     timerCounter = 0.0f;
    //     m_out = 0.0f;
    //     return;
    // }

    // timerCounter += timestep;
    // if (timerCounter > timeout) {
    //     if (repeat)
    //         timerCounter = 0.0f;
    //     else
    //         timerCounter = timeout;
    // }
    // float curveProgress = timerCounter / timeout;

// switch (curveType) {
// case Types::Toggle:
//     if (curveProgress > 0.5f)
//         m_out = 1.0f;
//     else
//         m_out = 0.0f;
//     break;
// case Types::Linear:
//     m_out = curveProgress;
//     break;
// case Types::Exponential:
//     m_out = curveProgress * curveProgress;
//     break;
// case Types::Breathing:
//     float gamma = 0.20f; // affects the width of peak (more or less darkness)
//     float beta = 0.5f;
//     m_out = (expf(-(powf((curveProgress - beta) / gamma, 2.0f)) / 2.0f)); //(exp ^ exp) / 2.0f;
//     break;
// }
