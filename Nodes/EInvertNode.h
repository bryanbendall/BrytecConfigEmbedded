#pragma once

#include "ENode.h"

namespace Brytec {

class EInvertNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t>
class EInvertNodeInternal : public EInvertNode {

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
        bool in = ToBool(m_in.getValue());

        if (!in) {
            m_out = 1.0f;
            return;
        }
        m_out = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Invert; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in;
#else
    ValueAndPointer m_in;
#endif
    float m_out;
};

}