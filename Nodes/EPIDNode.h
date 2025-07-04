#pragma once

#include "ENode.h"

namespace Brytec {

class EPIDNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t,
    typename Input2_t,
    typename Input3_t,
    typename Input4_t,
    typename Input5_t>
class EPIDNodeInternal : public EPIDNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_input.setPointer(output);
            break;
        case 1:
            m_target.setPointer(output);
            break;
        case 2:
            m_P.setPointer(output);
            break;
        case 3:
            m_I.setPointer(output);
            break;
        case 4:
            m_D.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_input.setValue(value);
            break;
        case 1:
            m_target.setValue(value);
            break;
        case 2:
            m_P.setValue(value);
            break;
        case 3:
            m_I.setValue(value);
            break;
        case 4:
            m_D.setValue(value);
            break;
#ifdef NODES_SIMULATION
        case 5:
            m_lastInput = value;
            break;
        case 6:
            m_iTerm = value;
            break;
#endif
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 5:
            return m_lastInput;
        case 6:
            return m_iTerm;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        // Adapted from:
        // http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

        static constexpr float outMax = 1.0f;
        static constexpr float outMin = 0.0f;

        float deltaTime = timestepMs / 1000.0f;
        float ki = (m_I * deltaTime);
        float kd = (m_D / deltaTime);

        /*Compute all the working error variables*/
        double error = m_target - m_input;
        m_iTerm += (ki * error);
        if (m_iTerm > outMax)
            m_iTerm = outMax;
        else if (m_iTerm < outMin)
            m_iTerm = outMin;
        double dInput = (m_input - m_lastInput);

        /*Compute PID Output*/
        m_out = m_P * error + m_iTerm - kd * dInput;
        if (m_out > outMax)
            m_out = outMax;
        else if (m_out < outMin)
            m_out = outMin;

        /*Remember some variables for next time*/
        m_lastInput = m_input;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::PID; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_input;
    ValueOrPointer<Input2_t> m_target;
    ValueOrPointer<Input3_t> m_P;
    ValueOrPointer<Input4_t> m_I;
    ValueOrPointer<Input5_t> m_D;
#else
    ValueAndPointer m_input;
    ValueAndPointer m_target;
    ValueAndPointer m_P;
    ValueAndPointer m_I;
    ValueAndPointer m_D;
#endif
    float m_lastInput;
    float m_iTerm;
    float m_out;
};
}