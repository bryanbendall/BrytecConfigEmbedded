#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EToggleNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EToggleNode); };

private:
    InputOrValue m_in;
    InputOrValue m_lastValue;
    float m_out;

    friend class ToggleNode;
};