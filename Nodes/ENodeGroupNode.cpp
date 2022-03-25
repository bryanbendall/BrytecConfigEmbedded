#include "ENodeGroupNode.h"

void ENodeGroupNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_rawData.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void ENodeGroupNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_rawData.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* ENodeGroupNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void ENodeGroupNode::Evaluate(float timestep)
{
    float& rawData = m_mask.isPointer(0) ? *m_rawData.pointer : m_rawData.value;

    if (rawData)
        m_out = rawData;
    else
        m_out = 0.0f;
}
