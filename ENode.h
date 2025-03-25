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
    CanBusInput,
    Convert,
    PinCurrent,
    PID,
    Counter,
    Color,
    Racepak_Switch_Panel,
    Holley_Broadcast,
    Interpolate,
    CanBusOutput,
    Thermistor,
    Holley_Io_Module,
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
    ConnectionType connections[10];

    uint8_t numValues = 0;
    float values[10];

    uint64_t uuid = 0;
};

static constexpr float FloatEpsilon = 0.0001f;

inline bool FloatToBool(float f)
{
    if (f >= FloatEpsilon)
        return true;
    else
        return false;
}

inline bool FloatIsEqual(float a, float b)
{
    return fabs((a - b)) <= FloatEpsilon;
}

inline uint32_t FloatToUint(float a)
{
    return (uint32_t)(a + FloatEpsilon);
}

inline uint32_t FloatTimeToMs(float time)
{
    return time * 1000.0f;
}

class ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    virtual void SetInput(uint8_t index, float* output) = 0;
    virtual void SetValue(uint8_t index, float value) = 0;
    virtual float GetValue(uint8_t index) = 0;
    virtual float* GetOutput(uint8_t index = 0) = 0;
    virtual void Evaluate(uint32_t timestepMs) = 0;
    virtual uint32_t Size() = 0;
    virtual NodeTypes NodeType() = 0;
};

}