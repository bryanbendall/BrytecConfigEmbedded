#pragma once

#include "ENode.h"

namespace Brytec {

class EMixColorNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Color1_t,
    typename Color2_t,
    typename Factor_t>
class EMixColorNodeInternal : public EMixColorNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_color1.setPointer(output);
            break;
        case 1:
            m_color2.setPointer(output);
            break;
        case 2:
            m_factor.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_color1.setValue(value);
            break;
        case 1:
            m_color2.setValue(value);
            break;
        case 2:
            m_factor.setValue(value);
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

        float r1 = ((uint32_t)m_color1 & 0xFF) * (1.0f - m_factor);
        float g1 = (((uint32_t)m_color1 >> 8) & 0xFF) * (1.0f - m_factor);
        float b1 = (((uint32_t)m_color1 >> 16) & 0xFF) * (1.0f - m_factor);

        float r2 = ((uint32_t)m_color2 & 0xFF) * m_factor;
        float g2 = (((uint32_t)m_color2 >> 8) & 0xFF) * m_factor;
        float b2 = (((uint32_t)m_color2 >> 16) & 0xFF) * m_factor;

        uint32_t outputColor = 0;
        outputColor |= (uint32_t)(r1 + r2 + 0.5f) << 0;
        outputColor |= (uint32_t)(g1 + g2 + 0.5f) << 8;
        outputColor |= (uint32_t)(b1 + b2 + 0.5f) << 16;

        m_out = outputColor;
    }

    uint32_t Size() override
    {
        return sizeof(*this);
    }

    NodeTypes NodeType() override { return NodeTypes::Mix_Color; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Color1_t> m_color1;
    ValueOrPointer<Color2_t> m_color2;
    ValueOrPointer<Factor_t> m_factor;
#else
    ValueAndPointer m_color1;
    ValueAndPointer m_color2;
    ValueAndPointer m_factor;
#endif
    float m_out;
};
}