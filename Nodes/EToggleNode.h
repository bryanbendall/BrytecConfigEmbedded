#pragma once

#include "BrytecConfigEmbedded/ENode.h"

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
            m_lastValue = value;
            break;
        }
    }
    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        bool in = ToBool(m_in.getValue());

        if (in) {
            if (ToBool(m_lastValue)) {
                return;
            } else {
                m_lastValue = true;
                if (ToBool(m_out))
                    m_out = 0.0f;
                else
                    m_out = 1.0f;
            }
        } else {
            m_lastValue = false;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    ValueOrPointer<Input1_t> m_in;
    float m_lastValue;
    float m_out;
};
