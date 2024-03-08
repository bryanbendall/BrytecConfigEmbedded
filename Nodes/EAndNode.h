#pragma once

#include "ENode.h"

namespace Brytec {

class EAndNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EAndNodeInternal : public EAndNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        if (index >= 5)
            return;

        m_inputs[index] = output;
    }

    void SetValue(uint8_t index, float value) override
    {
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
        m_out = 0.0f;

        for (auto* input : m_inputs) {
            if (input && !ToBool(*input))
                return;
        }

        m_out = 1.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::And; }

private:
    float* m_inputs[5] = {};
    float m_out;
};

}