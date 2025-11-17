#pragma once

#include "ENode.h"
#include <limits>

namespace Brytec {

enum class ChangeType : uint8_t {
    Rising,
    Falling,
    Both,
    Count
};

class EChangeNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <ChangeType type, typename Input1_t>
class EChangeNodeInternal : public EChangeNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_input.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_input.setValue(value);
            break;
        case 2:
            m_thershold = value;
            break;
        case 3:
            m_lastInput = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 1:
            return (float)type;
        case 2:
            return m_thershold;
        case 3:
            return m_lastInput;
        }
        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        m_out = 0.0f;

        // If last input has not been set yet
        if (m_lastInput == std::numeric_limits<float>::max()) {
            m_lastInput = m_out;
            return;
        }

        float difference = m_input - m_lastInput;

        if constexpr (type == ChangeType::Rising) {
            if (difference > m_thershold)
                m_out = 1.0f;
        }

        if constexpr (type == ChangeType::Falling) {
            difference = m_lastInput - m_input;
            if (difference > m_thershold)
                m_out = 1.0f;
        }

        if constexpr (type == ChangeType::Both) {
            difference = std::abs(difference);
            if (difference > m_thershold)
                m_out = 1.0f;
        }

        m_lastInput = m_input;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Change; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_input;
#else
    ValueAndPointer m_input;
#endif
    float m_thershold;
    float m_lastInput;
    float m_out;
};
}