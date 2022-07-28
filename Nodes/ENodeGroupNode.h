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
        switch (index) {
        case 0:
            m_simValue = value;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 0:
            return m_simValue;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        // TODO remove m_rawData?
        m_out = m_simValue;
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    float m_simValue;
    float m_out;
};