#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ECompareNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(ECompareNode); };

    enum class Types : uint8_t {
        Equal,
        NotEqual,
        Greater,
        GreaterEqual,
        Less,
        LessEqual,
        Count
    };

private:
    InputOrValue m_input1;
    InputOrValue m_input2;
    InputOrValue m_compareType;
    float m_out;

    const float m_epsilon = 0.0001;

    friend class CompareNode;
};