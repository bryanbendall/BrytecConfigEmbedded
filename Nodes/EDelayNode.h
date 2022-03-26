#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EDelayNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EDelayNode); };

private:
    InputOrValue m_in;
    InputOrValue m_delayTime;
    InputOrValue m_counter;
    float m_out;

    friend class DelayNode;
};