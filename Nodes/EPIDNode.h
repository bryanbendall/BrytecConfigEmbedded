#pragma once

#include "ENode.h"

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
        case 5:
            m_previousError = value;
            break;
        case 6:
            m_integral = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 5:
            return m_previousError;
        case 6:
            return m_integral;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        // timestep from seconds to milliseconds
        timestep *= 1000;

        float error = m_target.getValue() - m_input.getValue();
        float proportional = error;
        m_integral = m_integral + error * timestep;
        float derivative = (error - m_previousError) / timestep;
        m_out = m_P.getValue() * proportional + m_I.getValue() * m_integral + m_D.getValue() * derivative;
        m_previousError = error;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::PID; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_input;
    ValueOrPointer<Input2_t> m_target;
    ValueOrPointer<Input3_t> m_P;
    ValueOrPointer<Input4_t> m_I;
    ValueOrPointer<Input5_t> m_D;
#else
    ValueAndPointer m_in1;
    ValueAndPointer m_in2;
    ValueAndPointer m_in3;
    ValueAndPointer m_in4;
    ValueAndPointer m_in5;
#endif
    float m_previousError;
    float m_integral;
    float m_out;
};
