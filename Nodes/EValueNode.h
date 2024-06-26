#pragma once

#include "ENode.h"

namespace Brytec {

class EValueNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EValueNodeInternal : public EValueNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        m_out = value;
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 0:
            return m_out;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Value; }

private:
    float m_out;
};

}