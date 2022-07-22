#pragma once

#include "BrytecConfigEmbedded/ENode.h"
#include <math.h>

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
        case 3:
            m_timerCounter = value;
            break;
        }
    }
    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        bool in = ToBool(m_in.getValue());
        bool repeat = ToBool(m_repeat.getValue());
        float timeout = m_timeout.getValue();

        if (!in) {
            m_timerCounter = 0.0f;
            m_out = 0.0f;
            return;
        }

        m_timerCounter += timestep;
        if (m_timerCounter > timeout) {
            if (repeat)
                m_timerCounter = 0.0f;
            else
                m_timerCounter = timeout;
        }
        float curveProgress = m_timerCounter / timeout;

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

private:
    ValueOrPointer<Input1_t> m_in;
    ValueOrPointer<Input2_t> m_repeat;
    ValueOrPointer<Input3_t> m_timeout;
    float m_timerCounter;
    float m_out;
};