#pragma once

#include "ENode.h"

class ENodeGroupNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class ENodeGroupNodeInternal : public ENodeGroupNode {

public:
    ENodeGroupNodeInternal(uint8_t moduleAddress, uint8_t nodeGroupIndex)
        : m_moduleAddress(moduleAddress)
        , m_nodeGroupIndex(nodeGroupIndex)
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

    NodeTypes NodeType() override { return NodeTypes::Node_Group; }

    uint8_t getModuleAddress() { return m_moduleAddress; }

    uint8_t getNodeGroupIndex() { return m_nodeGroupIndex; }

private:
    uint8_t m_moduleAddress;
    uint8_t m_nodeGroupIndex;
    float m_out;
};