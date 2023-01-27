#pragma once

#include "ENode.h"

enum class CompareType : uint8_t {
    Equal,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    Count
};

class ECompareNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <CompareType type, typename Input1_t, typename Input2_t>
class ECompareNodeInternal : public ECompareNode {

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

    void Evaluate(float timestep) override
    {
        m_out = 0.0f;

        if constexpr (type == CompareType::Equal) {
            if (fabs(m_input1.getValue() - m_input2.getValue()) <= m_epsilon)
                m_out = 1.0f;
        }

        if constexpr (type == CompareType::NotEqual) {
            if (fabs(m_input1.getValue() - m_input2.getValue()) > m_epsilon)
                m_out = 1.0f;
        }

        if constexpr (type == CompareType::Greater) {
            if (m_input1.getValue() > m_input2.getValue())
                m_out = 1.0f;
        }

        if constexpr (type == CompareType::GreaterEqual) {
            if (m_input1.getValue() >= m_input2.getValue())
                m_out = 1.0f;
        }

        if constexpr (type == CompareType::Less) {
            if (m_input1.getValue() < m_input2.getValue())
                m_out = 1.0f;
        }

        if constexpr (type == CompareType::LessEqual) {
            if (m_input1.getValue() <= m_input2.getValue())
                m_out = 1.0f;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Compare; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_input1;
    ValueOrPointer<Input2_t> m_input2;
#else
    ValueAndPointer m_input1;
    ValueAndPointer m_input2;
#endif
    float m_out;

    static constexpr float m_epsilon = 0.0001;
};