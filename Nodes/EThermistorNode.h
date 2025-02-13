#pragma once

#include "ENode.h"

namespace Brytec {

class EThermistorNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EThermistorNodeInternal : public EThermistorNode {

    enum TempFormat : uint8_t {
        Celsius,
        Fahrenheit,
        Kelvin
    };

public:
    void SetInput(uint8_t index, float* output) override;
    void SetValue(uint8_t index, float value) override;
    float GetValue(uint8_t index) override;
    float* GetOutput(uint8_t index = 0) override;
    void Evaluate(uint32_t timestepMs) override;
    uint32_t Size() override { return sizeof(*this); }
    NodeTypes NodeType() override { return NodeTypes::And; }

private:
    void CalculateBeta();
    float ToKelvin(float value);
    float FromKelvin(float value);

private:
    float* m_input = nullptr;
    float m_res1 = 0.0f;
    float m_temp1 = 0.0f;
    float m_res2 = 0.0f;
    float m_temp2 = 0.0f;
    TempFormat m_format;
    float m_beta;
    float m_out;
};
}