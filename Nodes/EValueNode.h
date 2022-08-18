#pragma once

#include "ENode.h"

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

    void Evaluate(float timestep) override
    {
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    float m_out;
};
