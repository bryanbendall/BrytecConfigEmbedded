#pragma once

#include "ENode.h"

namespace Brytec {

class ESwitchNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t, typename Input2_t, typename Input3_t>
class ESwitchNodeInternal : public ESwitchNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_selection.setPointer(output);
            break;
        case 1:
            m_input1.setPointer(output);
            break;
        case 2:
            m_input2.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_selection.setValue(value);
            break;
        case 1:
            m_input1.setValue(value);
            break;
        case 2:
            m_input2.setValue(value);
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
        bool selection = ToBool(m_selection.getValue());
        float input1 = m_input1.getValue();
        float input2 = m_input2.getValue();

        if (selection)
            m_out = input1;
        else
            m_out = input2;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Switch; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_selection;
    ValueOrPointer<Input2_t> m_input1;
    ValueOrPointer<Input3_t> m_input2;
#else
    ValueAndPointer m_selection;
    ValueAndPointer m_input1;
    ValueAndPointer m_input2;
#endif
    float m_out;
};

}