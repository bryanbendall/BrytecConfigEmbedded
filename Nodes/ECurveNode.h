#pragma once

#include "ENode.h"
#include <math.h>

namespace Brytec {

enum class CurveType : uint8_t {
    Toggle,
    Linear,
    Exponential,
    Breathing,
    Count
};

class ECurveNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

private:
};

template <CurveType type,
    typename Input1_t,
    typename Input2_t,
    typename Input3_t>
class ECurveNodeInternal : public ECurveNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in.setPointer(output);
            break;
        case 1:
            m_repeat.setPointer(output);
            break;
        case 2:
            m_timeout.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_in.setValue(value);
            break;
        case 1:
            m_repeat.setValue(value);
            break;
        case 2:
            m_timeout.setValue(value);
            break;
#ifdef NODES_SIMULATION
        case 4:
            m_timerCounter = value;
            break;
#endif
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 3:
            return (float)type;
        case 4:
            return m_timerCounter;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        bool in = FloatToBool(m_in);
        bool repeat = FloatToBool(m_repeat);
        uint32_t timeout = FloatTimeToMs(m_timeout);

        if (!in) {
            m_timerCounter = 0;
            m_out = 0.0f;
            return;
        }

        m_timerCounter += timestepMs;
        if (m_timerCounter > timeout) {
            if (repeat)
                m_timerCounter = 0;
            else
                m_timerCounter = timeout;
        }
        float curveProgress = (float)m_timerCounter / (float)timeout;

        if constexpr (type == CurveType::Toggle) {
            if (curveProgress > 0.5f)
                m_out = 1.0f;
            else
                m_out = 0.0f;
        }

        if constexpr (type == CurveType::Linear) {
            m_out = curveProgress;
        }

        if constexpr (type == CurveType::Exponential) {
            m_out = curveProgress * curveProgress;
        }

        if constexpr (type == CurveType::Breathing) {
            float gamma = 0.20f; // affects the width of peak (more or less darkness)
            float beta = 0.5f;
            m_out = (expf(-(powf((curveProgress - beta) / gamma, 2.0f)) / 2.0f)); //(exp ^ exp) / 2.0f;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Curve; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in;
    ValueOrPointer<Input2_t> m_repeat;
    ValueOrPointer<Input3_t> m_timeout;
#else
    ValueAndPointer m_in;
    ValueAndPointer m_repeat;
    ValueAndPointer m_timeout;
#endif
    uint32_t m_timerCounter;
    float m_out;
};

}