#pragma once

#include "ENode.h"

namespace Brytec {

enum class InterpolateType : uint8_t {
    Linear,
    EaseInExpo,
    EaseOutExpo,
    EaseInOutExpo,
    EaseInCubic,
    EaseOutCubic,
    EaseInOutCubic,
    Count
};

class EInterpolateNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <InterpolateType type,
    typename Input1_t,
    typename Input2_t>
class EInterpolateNodeInternal : public EInterpolateNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in.setPointer(output);
            break;
        case 1:
            m_delayTime.setPointer(output);
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
            m_delayTime.setValue(value);
            break;
        case 3:
            m_timerCounter = value;
            break;
        case 4:
            m_out = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 2:
            return (float)type;
        case 3:
            return m_timerCounter;
        case 4:
            return m_out;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {

        float transitionTime = m_delayTime;

        float preTimer = m_timerCounter;
        m_timerCounter += timestep;

        if (m_timerCounter >= transitionTime || FloatIsEqual(m_in, m_out)) {
            m_timerCounter = 0.0f;
            m_out = m_in;

        } else {
            // End point
            float x1 = 1.0f;
            float y1 = m_in;

            // Previous point
            float x2 = preTimer / transitionTime;
            // Easing
            {
                if constexpr (type == InterpolateType::Linear) { }
                // Do not need to change for linear

                // Expo
                if constexpr (type == InterpolateType::EaseInExpo)
                    easeInExpo(x2);
                if constexpr (type == InterpolateType::EaseOutExpo)
                    easeOutExpo(x2);
                if constexpr (type == InterpolateType::EaseInOutExpo)
                    easeInOutExpo(x2);

                // Cubic
                if constexpr (type == InterpolateType::EaseInCubic)
                    easeInCubic(x2);
                if constexpr (type == InterpolateType::EaseOutCubic)
                    easeOutCubic(x2);
                if constexpr (type == InterpolateType::EaseInOutCubic)
                    easeInOutCubic(x2);
            }
            float y2 = m_out;

            // Line slope
            float m = (y2 - y1) / (x2 - x1);

            // Staring value
            float y = m * (0.0f - x1) + y1;

            // New t value
            float t = m_timerCounter / transitionTime;
            // Easing
            {
                if constexpr (type == InterpolateType::Linear) { }
                // Do not need to change for linear

                // Expo
                if constexpr (type == InterpolateType::EaseInExpo)
                    easeInExpo(t);
                if constexpr (type == InterpolateType::EaseOutExpo)
                    easeOutExpo(t);
                if constexpr (type == InterpolateType::EaseInOutExpo)
                    easeInOutExpo(t);

                // Cubic
                if constexpr (type == InterpolateType::EaseInCubic)
                    easeInCubic(t);
                if constexpr (type == InterpolateType::EaseOutCubic)
                    easeOutCubic(t);
                if constexpr (type == InterpolateType::EaseInOutCubic)
                    easeInOutCubic(t);
            }

            // Set output
            m_out = (m_in - y) * t + y;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Interpolate; }

    void easeInExpo(float& t)
    {
        if (!FloatIsEqual(t, 0.0f))
            t = powf(2.0f, 10.0f * t - 10.0f);
    }

    void easeOutExpo(float& t)
    {
        if (FloatIsEqual(t, 1.0f))
            t = 1.0f;
        else
            t = 1.0f - powf(2.0f, -10.0f * t);
    }

    void easeInOutExpo(float& t)
    {
        if (FloatIsEqual(t, 0.0f)) {
            t = 0.0f;
            return;
        }

        if (FloatIsEqual(t, 1.0f)) {
            t = 1.0f;
            return;
        }

        if (t < 0.5f)
            t = powf(2.0f, 20.0f * t - 10.0f) / 2.0f;
        else
            t = (2.0f - powf(2.0f, -20.0f * t + 10.0f)) / 2.0f;
    }

    void easeInCubic(float& t)
    {
        t = t * t * t;
    }

    void easeOutCubic(float& t)
    {
        t = 1.0f - powf(1.0f - t, 3.0f);
    }

    void easeInOutCubic(float& t)
    {
        if (t < 0.5f)
            t = 4.0f * t * t * t;
        else
            t = 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t>
        m_in;
    ValueOrPointer<Input2_t> m_delayTime;
#else
    ValueAndPointer m_in;
    ValueAndPointer m_delayTime;
#endif
    float m_timerCounter;
    float m_out;
};
}