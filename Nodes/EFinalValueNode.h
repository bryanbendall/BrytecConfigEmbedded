#pragma once

#include "ENode.h"

class EFinalValueNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t>
class EFinalValueNodeInternal : public EFinalValueNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_input1.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_input1.setValue(value);
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        // TODO revisit this to eliminate extra float
        m_out = m_input1.getValue();
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        // Nothing to do
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Final_Value; }

private:
    ValueOrPointer<Input1_t> m_input1;
    float m_out;
};