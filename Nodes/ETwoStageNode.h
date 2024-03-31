#pragma once

#include "ENode.h"

namespace Brytec {

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

    float GetValue(uint8_t index) override
    {
        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        bool stage1Trigger = FloatToBool(m_stage1Trigger);
        float stage1Percent = m_stage1Percent;
        bool stage2Trigger = FloatToBool(m_stage2Trigger);
        float stage2Percent = m_stage2Percent;

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

    NodeTypes NodeType() override { return NodeTypes::Two_Stage; }

private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Input1_t> m_stage1Trigger;
    ValueOrPointer<Input2_t> m_stage1Percent;
    ValueOrPointer<Input1_t> m_stage2Trigger;
    ValueOrPointer<Input2_t> m_stage2Percent;
#else
    ValueAndPointer m_stage1Trigger;
    ValueAndPointer m_stage1Percent;
    ValueAndPointer m_stage2Trigger;
    ValueAndPointer m_stage2Percent;
#endif
    float m_out;
};

}