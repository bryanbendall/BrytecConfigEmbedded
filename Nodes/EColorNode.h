#pragma once

#include "ENode.h"
#include <string.h>

namespace Brytec {

class EColorNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t,
    typename Input2_t,
    typename Input3_t>
class EColorNodeInternal : public EColorNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_red.setPointer(output);
            break;
        case 1:
            m_green.setPointer(output);
            break;
        case 2:
            m_blue.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_red.setValue(value);
            break;
        case 1:
            m_green.setValue(value);
            break;
        case 2:
            m_blue.setValue(value);
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
        // Clamp color channels
        float red = m_red;
        if (red < 0.0f)
            red = 0.0f;
        if (red > 1.0f)
            red = 1.0f;

        float green = m_green;
        if (green < 0.0f)
            green = 0.0f;
        if (green > 1.0f)
            green = 1.0f;

        float blue = m_blue;
        if (blue < 0.0f)
            blue = 0.0f;
        if (blue > 1.0f)
            blue = 1.0f;

        uint32_t outputColor = 0;
        outputColor |= (uint32_t)(red * 255.0f + 0.5f) << 0;
        outputColor |= (uint32_t)(green * 255.0f + 0.5f) << 8;
        outputColor |= (uint32_t)(blue * 255.0f + 0.5f) << 16;

        m_out = outputColor;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Color; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_red;
    ValueOrPointer<Input2_t> m_green;
    ValueOrPointer<Input3_t> m_blue;
#else
    ValueAndPointer m_red;
    ValueAndPointer m_green;
    ValueAndPointer m_blue;
#endif
    float m_out;
};
}