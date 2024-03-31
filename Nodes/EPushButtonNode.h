#pragma once

#include "ENode.h"

namespace Brytec {

class EPushButtonNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t, typename Input2_t, typename Input3_t>
class EPushButtonNodeInternal : public EPushButtonNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_button.setPointer(output);
            break;
        case 1:
            m_neutralSafety.setPointer(output);
            break;
        case 2:
            m_engineRunning.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_button.setValue(value);
            break;
        case 1:
            m_neutralSafety.setValue(value);
            break;
        case 2:
            m_engineRunning.setValue(value);
            break;
        case 3:
            m_lastButtonState = value;
            break;
        case 4:
            m_ignitionOut = value;
            break;
        case 5:
            m_starterOut = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 3:
            return m_lastButtonState;
        case 4:
            return m_ignitionOut;
        case 5:
            return m_starterOut;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        switch (index) {
        case 0:
            return &m_ignitionOut;
        case 1:
            return &m_starterOut;
        }

        return nullptr;
    }

    void Evaluate(float timestep) override
    {
        bool button = FloatToBool(m_button);
        bool neutralSafety = FloatToBool(m_neutralSafety);
        bool engineRunning = FloatToBool(m_engineRunning);

        if (engineRunning && FloatToBool(m_starterOut)) {
            m_starterOut = 0.0f;
            return;
        }

        if (FloatToBool(m_lastButtonState) == button) {
            return;
        }
        m_lastButtonState = button;

        if (button && !FloatToBool(m_ignitionOut)) {
            m_ignitionOut = 1.0f;
            m_starterOut = 0.0f;
            return;
        }

        if (button && !engineRunning && !neutralSafety) {
            m_ignitionOut = 0.0f;
            m_starterOut = 0.0f;
            return;
        }

        if (button && engineRunning) {
            m_ignitionOut = 0.0f;
            m_starterOut = 0.0f;
            return;
        }

        if (button && !engineRunning && neutralSafety) {
            m_starterOut = 1.0f;
            return;
        }

        m_starterOut = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Push_Button; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_button;
    ValueOrPointer<Input2_t> m_neutralSafety;
    ValueOrPointer<Input3_t> m_engineRunning;
#else
    ValueAndPointer m_button;
    ValueAndPointer m_neutralSafety;
    ValueAndPointer m_engineRunning;
#endif
    float m_lastButtonState;
    float m_ignitionOut;
    float m_starterOut;
};

}