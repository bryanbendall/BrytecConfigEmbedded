#pragma once

#include "ENode.h"

namespace Brytec {

enum class MathType : uint8_t {
    Add,
    Subtract,
    Multiply,
    Divide,
    Min,
    Max,
    Count
};

class EMathNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <MathType type, typename Input1_t, typename Input2_t>
class EMathNodeInternal : public EMathNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_input1.setPointer(output);
            break;
        case 1:
            m_input2.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_input1.setValue(value);
            break;
        case 1:
            m_input2.setValue(value);
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 2:
            return (float)type;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        if constexpr (type == MathType::Add)
            m_out = m_input1 + m_input2;

        if constexpr (type == MathType::Subtract)
            m_out = m_input1 - m_input2;

        if constexpr (type == MathType::Multiply)
            m_out = m_input1 * m_input2;

        if constexpr (type == MathType::Divide)
            m_out = m_input1 / m_input2;

        if constexpr (type == MathType::Min)
            m_out = m_input1 > m_input2 ? m_input2 : m_input1;

        if constexpr (type == MathType::Max)
            m_out = m_input1 < m_input2 ? m_input2 : m_input1;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Math; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_input1;
    ValueOrPointer<Input2_t> m_input2;
#else
    ValueAndPointer m_input1;
    ValueAndPointer m_input2;
#endif
    float m_out;
};

}