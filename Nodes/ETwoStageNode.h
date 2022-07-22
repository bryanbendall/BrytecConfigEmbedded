#pragma once

#include "BrytecConfigEmbedded/ENode.h"

class ETwoStageNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t, typename Input2_t, typename Input3_t, typename Input4_t>
class ETwoStageNodeInternal : public ETwoStageNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_stage1Trigger.setPointer(output);
            break;
        case 1:
            m_stage1Percent.setPointer(output);
            break;
        case 2:
            m_stage2Trigger.setPointer(output);
            break;
        case 3:
            m_stage2Percent.setPointer(output);
            break;
        }
    }
    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_stage1Trigger.setValue(value);
            break;
        case 1:
            m_stage1Percent.setValue(value);
            break;
        case 2:
            m_stage2Trigger.setValue(value);
            break;
        case 3:
            m_stage2Percent.setValue(value);
            break;
        }
    }
    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        bool stage1Trigger = ToBool(m_stage1Trigger.getValue());
        float stage1Percent = m_stage1Percent.getValue();
        bool stage2Trigger = ToBool(m_stage2Trigger.getValue());
        float stage2Percent = m_stage2Percent.getValue();

        if (stage2Trigger) {
            m_out = stage2Percent;
            return;
        } else if (stage1Trigger) {
            m_out = stage1Percent;
            return;
        }
        m_out = 0.0f;
    }

    uint32_t Size() override { return sizeof(*this); }

private:
    ValueOrPointer<Input1_t> m_stage1Trigger;
    ValueOrPointer<Input2_t> m_stage1Percent;
    ValueOrPointer<Input1_t> m_stage2Trigger;
    ValueOrPointer<Input2_t> m_stage2Percent;
    float m_out;
};
