#pragma once

#include "EBrytecConfig.h"
#include "Utils/ENodeConnection.h"
#include "Utils/PlacementNew.h"
#include <math.h>
#include <stdint.h>

namespace Brytec {

enum class NodeTypes : uint8_t {
    Final_Value = 0,
    Initial_Value,
    Node_Group,
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
    PinCurrent,
    PID,
    Counter,
    Count
};

struct ENodeConnection {
    int16_t connectionNodeIndex = -1;
    int8_t outputIndex = -1;
    float defaultValue = 0.0f;
};

struct ENodeSpec {
    NodeTypes type;

    uint8_t numInputs = 0;
    ConnectionType connections[8];

    uint8_t numValues = 0;
    float values[8];

    uint8_t moduleAddress = 0;
    uint16_t pinIndex = 0;
};

inline bool ToBool(float f)
{
    if (f >= 0.0001f)
        return true;
    else
        return false;
}

inline bool isEqual(float a, float b)
{
    return fabs((a - b)) <= 0.0001f;
}

class ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    virtual void SetInput(uint8_t index, float* output) = 0;
    virtual void SetValue(uint8_t index, float value) = 0;
    virtual float GetValue(uint8_t index) = 0;
    virtual float* GetOutput(uint8_t index = 0) = 0;
    virtual void Evaluate(float timestep) = 0;
    virtual uint32_t Size() = 0;
    virtual NodeTypes NodeType() = 0;
};

}