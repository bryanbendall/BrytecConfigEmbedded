#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class EDelayNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t, typename Input2_t>
class EDelayNodeInternal : public EDelayNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in.setPointer(output);
            break;
        case 1:
            m_delayTime.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_in.setValue(value);
            break;
        case 1:
            m_delayTime.setValue(value);
            break;
        case 2:
            m_timerCounter = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 2:
            return m_timerCounter;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        bool in = ToBool(m_in.getValue());
        float delayTime = m_delayTime.getValue();

        if (in) {
            m_timerCounter += timestep;
            if (m_timerCounter >= delayTime) {
                m_out = in;
                m_timerCounter = delayTime;
            }
        } else {
            m_timerCounter = 0.0f;
            m_out = 0.0f;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    ValueOrPointer<Input1_t> m_in;
    ValueOrPointer<Input2_t> m_delayTime;
    float m_timerCounter;
    float m_out;
};
