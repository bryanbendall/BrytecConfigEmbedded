#include "ETwoStageNode.h"

void ETwoStageNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_stage1Trigger.pointer = output;
        m_mask.setPointer(index);
        break;
    case 1:
        m_stage1Percent.pointer = output;
        m_mask.setPointer(index);
        break;
    case 2:
        m_stage2Trigger.pointer = output;
        m_mask.setPointer(index);
        break;
    case 3:
        m_stage2Percent.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void ETwoStageNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_stage1Trigger.value = value;
        m_mask.setValue(index);
        break;
    case 1:
        m_stage1Percent.value = value;
        m_mask.setValue(index);
        break;
    case 2:
        m_stage2Trigger.value = value;
        m_mask.setValue(index);
        break;
    case 3:
        m_stage2Percent.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* ETwoStageNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void ETwoStageNode::Evaluate(float timestep)
{
    bool stage1Trigger = ToBool(m_mask.isPointer(0) ? *m_stage1Trigger.pointer : m_stage1Trigger.value);
    float& stage1Percent = m_mask.isPointer(1) ? *m_stage1Percent.pointer : m_stage1Percent.value;
    bool stage2Trigger = ToBool(m_mask.isPointer(2) ? *m_stage2Trigger.pointer : m_stage2Trigger.value);
    float& stage2Percent = m_mask.isPointer(3) ? *m_stage2Percent.pointer : m_stage2Percent.value;

    if (stage2Trigger) {
        m_out = stage2Percent;
        return;
    } else if (stage1Trigger) {
        m_out = stage1Percent;
        return;
    }
    m_out = 0.0f;
}
