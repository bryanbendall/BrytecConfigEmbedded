#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EMathNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EMathNode); };

    enum class Types : uint8_t {
        Add,
        Subtract,
        Multiply,
        Divide,
        Count
    };

private:
    InputOrValue m_input1;
    InputOrValue m_input2;
    InputOrValue m_mathType;
    float m_out;

    friend class MathNode;
};