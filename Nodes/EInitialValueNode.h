#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EInitialValueNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EInitialValueNodeInternal : public EInitialValueNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        // Special case for input value
        m_out = value;
    }
    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    float m_out;
};