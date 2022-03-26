#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EInvertNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EInvertNode); };

private:
    InputOrValue m_in;
    float m_out;

    friend class InvertNode;
};