#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EFinalValueNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(EFinalValueNode); };

private:
    // InputOrValue m_in;

    friend class FinalValueNode;
};