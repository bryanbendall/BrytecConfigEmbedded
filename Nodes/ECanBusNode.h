#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ECanBusNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float GetValue(uint8_t index) override
    {
        return 0.0f;
    }
    float* GetOutput(uint8_t index) override;
    void Evaluate(float timestep) override;
    uint32_t Size() override { return sizeof(ECanBusNode); };

private:
    float id;
    float data0;
    float data1;
    float data2;
    float data3;
    float data4;
    float data5;
    float data6;
    float data7;

    friend class CanBusNode;
};