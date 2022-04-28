#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ESwitchNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(ESwitchNode); };

private:
    // InputOrValue m_selection;
    // InputOrValue m_input1;
    // InputOrValue m_input2;
    float m_out;

    friend class SwitchNode;
};