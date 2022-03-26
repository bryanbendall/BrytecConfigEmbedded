#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EOnOffNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EOnOffNode); };

private:
    InputOrValue m_on;
    InputOrValue m_off;
    float m_out;

    friend class OnOffNode;
};