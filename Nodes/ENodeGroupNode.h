#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ENodeGroupNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class ENodeGroupNodeInternal : public ENodeGroupNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        m_rawData = value;
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
        // TODO remove m_rawData?
        m_out = m_rawData;
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    float m_rawData;
    float m_out;
};