#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EAndNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EAndNode); };

private:
    // InputOrValue m_in1 = { 1.0f };
    // InputOrValue m_in2 = { 1.0f };
    // InputOrValue m_in3 = { 1.0f };
    // InputOrValue m_in4 = { 1.0f };
    // InputOrValue m_in5 = { 1.0f };
    float m_out;

    friend class AndNode;
};