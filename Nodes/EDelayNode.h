#pragma once

#include "ENode.h"

namespace Brytec {

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
#ifdef NODES_SIMULATION
        case 2:
            m_timerCounter = value;
            break;
#endif
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

    void Evaluate(uint32_t timestepMs) override
    {
        bool in = FloatToBool(m_in);
        uint32_t delayTime = FloatTimeToMs(m_delayTime);

        if (in) {
            m_timerCounter += timestepMs;
            if (m_timerCounter >= delayTime) {
                m_out = m_in;
                m_timerCounter = delayTime;
            }
        } else {
            m_timerCounter = 0;
            m_out = 0.0f;
        }
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Delay; }

private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_in;
    ValueOrPointer<Input2_t> m_delayTime;
#else
    ValueAndPointer m_in;
    ValueAndPointer m_delayTime;
#endif
    uint32_t m_timerCounter;
    float m_out;
};

}