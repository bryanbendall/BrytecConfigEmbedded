#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EPushButtonNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EPushButtonNode); };

private:
    InputOrValue m_button;
    InputOrValue m_neutralSafety;
    InputOrValue m_engineRunning;
    InputOrValue m_lastButtonState;
    float m_ignitionOut;
    float m_starterOut;

    friend class PushButtonNode;
};