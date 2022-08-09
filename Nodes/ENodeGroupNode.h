#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ENodeGroupNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class ENodeGroupNodeInternal : public ENodeGroupNode {

public:
    ENodeGroupNodeInternal(uint8_t moduleAddress, uint8_t pinIndex)
        : m_moduleAddress(moduleAddress)
        , m_pinIndex(pinIndex)
    {
    }

    void
    SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_out = value;
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
    uint8_t getModuleAddress() { return m_moduleAddress; }
    uint8_t getPinIndex() { return m_pinIndex; }

private:
    uint8_t m_moduleAddress;
    uint8_t m_pinIndex;
    float m_out;
};