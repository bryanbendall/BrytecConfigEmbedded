#pragma once

#include "BrytecConfigEmbedded/ENode.h"

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
        }
    }
    float* GetOutput(uint8_t index = 0) override
    {
        switch (index) {
        case 0:
            return &m_ignitionOut;
            break;
        case 1:
            return &m_starterOut;
            break;
        }

        return nullptr;
    }

    void Evaluate(float timestep) override
    {
        bool button = ToBool(m_button.getValue());
        bool neutralSafety = ToBool(m_neutralSafety.getValue());
        bool engineRunning = ToBool(m_engineRunning.getValue());

        if (engineRunning && ToBool(m_starterOut)) {
            m_starterOut = 0.0f;
            return;
        }

        if (ToBool(m_lastButtonState) == button) {
            return;
        }
        m_lastButtonState = button;

        if (button && !ToBool(m_ignitionOut)) {
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

private:
    ValueOrPointer<Input1_t> m_button;
    ValueOrPointer<Input2_t> m_neutralSafety;
    ValueOrPointer<Input3_t> m_engineRunning;
    float m_lastButtonState;
    float m_ignitionOut;
    float m_starterOut;
};