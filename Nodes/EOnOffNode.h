#pragma once

#include "ENode.h"

namespace Brytec {

class EOnOffNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t, typename Input2_t>
class EOnOffNodeInternal : public EOnOffNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_on.setPointer(output);
            break;
        case 1:
            m_off.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_on.setValue(value);
            break;
        case 1:
            m_off.setValue(value);
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
        bool on = FloatToBool(m_on);
        bool off = FloatToBool(m_off);

        if (off) {
            m_out = 0.0f;
            return;
        } else if (on) {
            m_out = 1.0f;
            return;
        }
        m_out = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::On_Off; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_on;
    ValueOrPointer<Input2_t> m_off;
#else
    ValueAndPointer m_on;
    ValueAndPointer m_off;
#endif
    float m_out;
};

}