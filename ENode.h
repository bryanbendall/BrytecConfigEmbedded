#pragma once

#include "ENodeConnection.h"
// #include "utils/BinarySerializer.h"
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

bool ToBool(float& f);

class ENode {

public:
    static ENode* CreateInPlace(NodeTypes type, /*BinaryDeserializer& des,*/ uint8_t* destination);

    virtual void SetInput(uint8_t index, float* output) = 0;
    virtual void SetValue(uint8_t index, float value) = 0;
    virtual float* GetOutput(uint8_t index = 0) = 0;
    virtual void Evaluate(float timestep) = 0;
    virtual uint32_t Size() = 0;
};