#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EValueNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EValueNode); };

private:
    float m_out;

    friend class ValueNode;
};