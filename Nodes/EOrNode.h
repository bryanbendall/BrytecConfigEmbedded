#pragma once

#include "ENode.h"

namespace Brytec {

class EOrNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t,
    typename Input2_t,
    typename Input3_t,
    typename Input4_t,
    typename Input5_t>
class EOrNodeInternal : public EOrNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in1.setPointer(output);
            break;
        case 1:
            m_in2.setPointer(output);
            break;
        case 2:
            m_in3.setPointer(output);
            break;
        case 3:
            m_in4.setPointer(output);
            break;
        case 4:
            m_in5.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_in1.setValue(value);
            break;
        case 1:
            m_in2.setValue(value);
            break;
        case 2:
            m_in3.setValue(value);
            break;
        case 3:
            m_in4.setValue(value);
            break;
        case 4:
            m_in5.setValue(value);
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        m_out = 1.0f;

        if (m_in1.getValue())
            return;
        if (m_in2.getValue())
            return;
        if (m_in3.getValue())
            return;
        if (m_in4.getValue())
            return;
        if (m_in5.getValue())
            return;

        m_out = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Or; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in1;
    ValueOrPointer<Input2_t> m_in2;
    ValueOrPointer<Input3_t> m_in3;
    ValueOrPointer<Input4_t> m_in4;
    ValueOrPointer<Input5_t> m_in5;
#else
    ValueAndPointer m_in1;
    ValueAndPointer m_in2;
    ValueAndPointer m_in3;
    ValueAndPointer m_in4;
    ValueAndPointer m_in5;
#endif
    float m_out;
};

}