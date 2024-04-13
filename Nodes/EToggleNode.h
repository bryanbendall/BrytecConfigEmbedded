#pragma once

#include "ENode.h"

namespace Brytec {

class EToggleNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t>
class EToggleNodeInternal : public EToggleNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_in.setValue(value);
            break;
#ifdef NODES_SIMULATION
        case 1:
            m_lastIn = value;
            break;
        case 2:
            m_lastOut = value;
            m_out = value;
            break;
#endif
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 1:
            return m_lastIn;
        case 2:
            return m_lastOut;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {

        if (m_in == m_lastIn) {
            return;
        }

        if (m_in) {
            if (m_lastOut)
                m_out = 0.0f;
            else
                m_out = 1.0f;

            m_lastOut = m_out;
            m_lastIn = true;
            return;
        }

        m_lastIn = false;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Toggle; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in;
#else
    ValueAndPointer m_in;
#endif
    bool m_lastIn;
    bool m_lastOut;
    float m_out;
};

}