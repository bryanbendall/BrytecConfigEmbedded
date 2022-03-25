#include "EValueNode.h"

void EValueNode::SetInput(uint8_t index, float* output)
{
    // None
}

void EValueNode::SetValue(uint8_t index, float value)
{
    // Special case because it is a static value
    if (index == 0)
        m_out = value;
}

float* EValueNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EValueNode::Evaluate(float timestep)
{
    // Nothing to do because its a static value
}
