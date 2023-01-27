#pragma once

#include "ENode.h"

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
        case 4:
            m_lastUp = value;
            break;
        case 5:
            m_lastDown = value;
            break;
        case 6:
            m_out = value;
            break;
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

    void Evaluate(float timestep) override
    {
        bool up = ToBool(m_up.getValue());
        bool down = ToBool(m_down.getValue());

        if (ToBool(m_lastUp) != up) {
            m_lastUp = up;
            if (up && !isEqual(m_out, m_max.getValue()))
                m_out++;
        }

        if (ToBool(m_lastDown) != down) {
            m_lastDown = down;
            if (down && !isEqual(m_out, m_min.getValue()))
                m_out--;
        }
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
    float m_lastUp;
    float m_lastDown;
    float m_out;
};