#pragma once

#include "ENode.h"

namespace Brytec {

class EClampNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input_t,
    typename Min_t,
    typename Max_t>
class EClampNodeInternal : public EClampNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_input.setPointer(output);
            break;
        case 1:
            m_min.setPointer(output);
            break;
        case 2:
            m_max.setPointer(output);
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
            m_min.setValue(value);
            break;
        case 2:
            m_max.setValue(value);
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

    void Evaluate(uint32_t timestepMs) override
    {
        if (m_input < m_min)
            m_out = m_min;
        else if (m_input > m_max)
            m_out = m_max;
        else
            m_out = m_input;
    }

    uint32_t Size() override
    {
        return sizeof(*this);
    }

    NodeTypes NodeType() override { return NodeTypes::Clamp; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input_t> m_input;
    ValueOrPointer<Min_t> m_min;
    ValueOrPointer<Max_t> m_max;
#else
    ValueAndPointer m_input;
    ValueAndPointer m_min;
    ValueAndPointer m_max;
#endif
    float m_out;
};
}