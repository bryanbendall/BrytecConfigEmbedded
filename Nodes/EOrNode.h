#pragma once

#include "ENode.h"

namespace Brytec {

class EOrNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EOrNodeInternal : public EOrNode {

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

    void Evaluate(uint32_t timestepMs) override
    {
        m_out = 1.0f;

        for (auto* input : m_inputs) {
            if (input && FloatToBool(*input))
                return;
        }

        m_out = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Or; }

private:
    float* m_inputs[5] = {};
    float m_out;
};
}