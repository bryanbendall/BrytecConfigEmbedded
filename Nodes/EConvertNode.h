#pragma once

#include "ENode.h"

class EConvertNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float GetValue(uint8_t index) override
    {
        return 0.0f;
    }
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EConvertNode); };

private:
    float m_out;

    friend class ConvertNode;
};