#pragma once

#include <cstddef>
#include <stdint.h>

enum class NodeTypes : uint8_t {
    Final_Value = 0,
    Initial_Value,
    Node_Group, // TODO
    And,
    Or,
    Two_Stage,
    Curve,
    Compare,
    On_Off,
    Invert,
    Toggle,
    Delay,
    Push_Button,
    Map_Value,
    Math,
    Value,
    Switch,
    CanBus,
    Convert,
    Count
};

union InputOrValue {
    float value;
    float* pointer;
};

bool ToBool(float& f);

class NodeInputMask {
    uint32_t m_mask;

public:
    void setPointer(uint8_t index) { m_mask |= (1 << index); }
    void setValue(uint8_t index) { m_mask &= ~(1 << index); }
    bool isPointer(uint8_t index) { return m_mask & (1 << index); }
};

class ENode {

public:
    static ENode* create(NodeTypes type, void* ptr);
    static uint32_t getSize(NodeTypes type) { return m_sizes[(size_t)type]; }

    virtual void SetInput(uint8_t index, float* output) = 0;
    virtual void SetValue(uint8_t index, float value) = 0;
    virtual float* GetOutput(uint8_t index) = 0;
    virtual void Evaluate(float timestep) = 0;
    virtual uint32_t Size() = 0;

protected:
    NodeInputMask m_mask;
    static const uint32_t m_sizes[(size_t)NodeTypes::Count];
};