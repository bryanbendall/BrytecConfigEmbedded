#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ECurveNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(ECurveNode); };

private:
    enum class Types : uint8_t {
        Toggle,
        Linear,
        Exponential,
        Breathing,
        Count
    };
    InputOrValue m_in;
    InputOrValue m_repeat;
    InputOrValue m_timeout;
    InputOrValue m_curveType;
    InputOrValue m_timerCounter;
    float m_out;
};