#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EOrNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EOrNode); };

private:
    // InputOrValue m_in1;
    // InputOrValue m_in2;
    // InputOrValue m_in3;
    // InputOrValue m_in4;
    // InputOrValue m_in5;
    float m_out;

    friend class OrNode;
};