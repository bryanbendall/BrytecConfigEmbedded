#pragma once

#include "ENode.h"

namespace Brytec {

class ERacepakSwitchPanelNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class ERacepakSwitchPanelNodeInternal : public ERacepakSwitchPanelNode {

#define RACEPAK_SWITCH_PANEL_NUM_INPUTS 8

public:
    void SetInput(uint8_t index, float* output) override
    {
        if (index >= RACEPAK_SWITCH_PANEL_NUM_INPUTS)
            return;

        m_in[index].setPointer(output);
    }

    void SetValue(uint8_t index, float value) override
    {
        if (index >= RACEPAK_SWITCH_PANEL_NUM_INPUTS)
            return;

        m_in[index].setValue(value);
    }

    float GetValue(uint8_t index) override
    {
        if (index >= RACEPAK_SWITCH_PANEL_NUM_INPUTS)
            return 0.0f;

        return m_in[index];
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return nullptr;
    }

    void Evaluate(float timestep) override
    {
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Racepak_Switch_Panel; }

private:
    ValueAndPointer m_in[RACEPAK_SWITCH_PANEL_NUM_INPUTS];
};
}