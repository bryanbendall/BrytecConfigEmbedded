#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ETwoStageNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(ETwoStageNode); };

private:
    // InputOrValue m_stage1Trigger;
    // InputOrValue m_stage1Percent;
    // InputOrValue m_stage2Trigger;
    // InputOrValue m_stage2Percent;
    float m_out;

    friend class TwoStageNode;
};