#pragma once

#include "ENode.h"

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
        case 1:
            m_lastIn = value;
            break;
        case 2:
            m_lastOut = value;
            m_out = value;
            break;
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

    void Evaluate(float timestep) override
    {

        if (ToBool(m_in.getValue()) == ToBool(m_lastIn)) {
            return;
        }

        if (ToBool(m_in.getValue())) {
            if (ToBool(m_lastOut))
                m_out = 0.0f;
            else
                m_out = 1.0f;

            m_lastOut = m_out;
            m_lastIn = 1.0f;
            return;
        }

        m_lastIn = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Toggle; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in;
#else
    ValueAndPointer m_in;
#endif
    float m_lastIn;
    float m_lastOut;
    float m_out;
};
