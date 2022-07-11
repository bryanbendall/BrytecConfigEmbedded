#pragma once

#include "ENodeConnection.h"
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

struct ENodeConnection {
    uint8_t connectionNodeIndex = -1;
    uint8_t outputIndex = -1;
    float defaultValue = 0.0f;
    ConnectionType type = ConnectionType::Float;
};

struct ENodeSpec {
    NodeTypes type;

    uint8_t numInputs = 0;
    ENodeConnection connections[20];

    uint8_t numValues = 0;
    float values[20];
};

bool ToBool(float& f);

class ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    virtual void SetInput(uint8_t index, float* output) = 0;
    virtual void SetValue(uint8_t index, float value) = 0;
    virtual float* GetOutput(uint8_t index = 0) = 0;
    virtual void Evaluate(float timestep) = 0;
    virtual uint32_t Size() = 0;
};