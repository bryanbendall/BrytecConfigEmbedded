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

        if (value > 0.0f) {
            int b = 0;
        }
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

    void Evaluate(float timestep) override
    {
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    float m_out;
};