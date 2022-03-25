#include "EInitialValueNode.h"

void EInitialValueNode::SetInput(uint8_t index, float* output)
{
    if (index == 0)
        m_rawData.pointer = output;
    m_mask.setPointer(index);
}

void EInitialValueNode::SetValue(uint8_t index, float value)
{
    if (index == 0)
        m_rawData.value = value;
    m_mask.setValue(index);
}

float* EInitialValueNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EInitialValueNode::Evaluate(float timestep)
{
    float rawData = m_mask.isPointer(0) ? *m_rawData.pointer : m_rawData.value;

    m_out = rawData;
}
