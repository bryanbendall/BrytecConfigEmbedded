#pragma once

#include "ENode.h"

namespace Brytec {

class ECounterNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

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
        case 4:
            m_loop.setPointer(output);
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
        case 4:
            m_loop.setValue(value);
            break;
#ifdef NODES_SIMULATION
        case 5:
            m_lastUp = value;
            break;
        case 6:
            m_lastDown = value;
            break;
        case 7:
            m_out = value;
            break;
#endif
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 5:
            return m_lastUp;
        case 6:
            return m_lastDown;
        case 7:
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
        bool loop = FloatToBool(m_loop);

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

        // Clamp or loop
        if (m_out > m_max) {
            if (loop)
                m_out = m_min;
            else
                m_out = m_max;
        }

        if (m_out < m_min) {
            if (loop)
                m_out = m_max;
            else
                m_out = m_min;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Counter; }

private:
    ValueAndPointer m_up;
    ValueAndPointer m_down;
    ValueAndPointer m_min;
    ValueAndPointer m_max;
    ValueAndPointer m_loop;
    bool m_lastUp;
    bool m_lastDown;
    float m_out;
};

}