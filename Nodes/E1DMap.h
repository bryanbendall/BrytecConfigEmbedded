#pragma once

#include "ENode.h"

namespace Brytec {

static constexpr inline uint8_t EOneDMapSize = 8;

template <typename Input1_t>
class EOneDMapNode : public ENode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        if (index == 0) {
            m_in.setValue(value);
        } else if (index < (EOneDMapSize + 1)) {
            m_inputMap[index - 1] = value;
        } else if (index < (EOneDMapSize * 2 + 1)) {
            m_outputMap[index - (EOneDMapSize + 1)] = value;
        }
    }

    float GetValue(uint8_t index) override
    {
        if (index == 0) {
            return m_in;
        } else if (index < EOneDMapSize + 1) {
            return m_inputMap[index - 1];
        } else if (index < (EOneDMapSize * 2 + 1)) {
            return m_outputMap[index - (EOneDMapSize + 1)];
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        uint8_t point1Index = 0;
        uint8_t point2Index = 0;

        // Stop early so we don't over run the array
        for (uint8_t i = 0; i < EOneDMapSize - 1; i++) {

            point1Index = i;
            point2Index = i + 1;

            if (m_in < m_inputMap[i + 1])
                break;
        }

        float m = (m_outputMap[point2Index] - m_outputMap[point1Index]) / (m_inputMap[point2Index] - m_inputMap[point1Index]);
        float b = m_outputMap[point1Index] - (m * m_inputMap[point1Index]);
        m_out = (m * m_in) + b;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::OneDMap; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in;
#else
    ValueAndPointer m_in;
#endif
    float m_inputMap[EOneDMapSize];
    float m_outputMap[EOneDMapSize];
    float m_out;
};

}