#pragma once

#include "ENode.h"

namespace Brytec {

class ECounterNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t, typename Input2_t, typename Input3_t, typename Input4_t>
class ECounterNodeInternal : public ECounterNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_up.setPointer(output);
            break;
        case 1:
            m_down.setPointer(output);
            break;
        case 2:
            m_min.setPointer(output);
            break;
        case 3:
            m_max.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_up.setValue(value);
            break;
        case 1:
            m_down.setValue(value);
            break;
        case 2:
            m_min.setValue(value);
            break;
        case 3:
            m_max.setValue(value);
            break;
#ifdef NODES_SIMULATION
        case 4:
            m_lastUp = value;
            break;
        case 5:
            m_lastDown = value;
            break;
        case 6:
            m_out = value;
            break;
#endif
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 4:
            return m_lastUp;
        case 5:
            return m_lastDown;
        case 6:
            return m_out;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        bool up = FloatToBool(m_up);
        bool down = FloatToBool(m_down);

        if (m_lastUp != up) {
            m_lastUp = up;
            if (up)
                m_out++;
        }

        if (m_lastDown != down) {
            m_lastDown = down;
            if (down)
                m_out--;
        }

        // Clamp
        if (m_out >= m_max)
            m_out = m_max;

        if (m_out <= m_min)
            m_out = m_min;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Counter; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_up;
    ValueOrPointer<Input2_t> m_down;
    ValueOrPointer<Input3_t> m_min;
    ValueOrPointer<Input4_t> m_max;
#else
    ValueAndPointer m_up;
    ValueAndPointer m_down;
    ValueAndPointer m_min;
    ValueAndPointer m_max;
#endif
    bool m_lastUp;
    bool m_lastDown;
    float m_out;
};

}