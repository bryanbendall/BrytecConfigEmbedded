#include "EFinalValueNode.h"

void EFinalValueNode::SetInput(uint8_t index, float* output)
{
    if (index == 0)
        m_in.pointer = output;
    m_mask.setPointer(index);
}

void EFinalValueNode::SetValue(uint8_t index, float value)
{
    if (index == 0)
        m_in.value = value;
    m_mask.setValue(index);
}

float* EFinalValueNode::GetOutput(uint8_t index)
{
    // No outputs
    return nullptr;
}

void EFinalValueNode::Evaluate(float timestep)
{
    // Nothing to do
}
