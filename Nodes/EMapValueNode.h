#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EMapValueNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EMapValueNode); };

private:
    InputOrValue m_in;
    InputOrValue m_fromMin;
    InputOrValue m_fromMax;
    InputOrValue m_toMin;
    InputOrValue m_toMax;
    float m_out;
};