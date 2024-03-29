#pragma once

#include "ENode.h"

namespace Brytec {

class ENodeGroupNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    virtual uint8_t getModuleAddress() = 0;

    virtual uint8_t getNodeGroupIndex() = 0;
};

template <typename Input1_t, typename Input2_t>
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
        switch (index) {
        case 0:
            m_defaultValue.setPointer(output);
            break;
        case 1:
            m_timeout.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_defaultValue.setValue(value);
            break;
        case 1:
            m_timeout.setValue(value);
            break;
        case 2:
            // Simulation value
            m_out = value;
            break;
        case 99:
            m_timeSinceLastUpdate = 0.0f;
            m_out = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 2:
            // Simulation value
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
        if (m_timeSinceLastUpdate >= m_timeout.getValue())
            m_out = m_defaultValue.getValue();

        m_timeSinceLastUpdate += timestep;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Node_Group; }

    uint8_t getModuleAddress() override { return m_moduleAddress; }

    uint8_t getNodeGroupIndex() override { return m_nodeGroupIndex; }

private:
    uint8_t m_moduleAddress;
    uint8_t m_nodeGroupIndex;
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_defaultValue;
    ValueOrPointer<Input2_t> m_timeout;
#else
    ValueAndPointer m_defaultValue;
    ValueAndPointer m_timeout;
#endif
    float m_timeSinceLastUpdate = 0.0f;
    float m_out;
};

}