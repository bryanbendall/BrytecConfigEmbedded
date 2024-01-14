#pragma once

#include "ENode.h"

namespace Brytec {

class EHolleyBroadcastNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EHolleyBroadcastNodeInternal : public EHolleyBroadcastNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        m_channel = value;
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 0:
            return m_channel;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Holley_Broadcast; }

private:
    uint32_t m_channel;
    float m_out;
};
}