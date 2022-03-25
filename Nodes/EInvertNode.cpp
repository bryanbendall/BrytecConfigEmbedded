#include "EInvertNode.h"

void EInvertNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_in.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void EInvertNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_in.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* EInvertNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EInvertNode::Evaluate(float timestep)
{
    bool in = ToBool(m_mask.isPointer(0) ? *m_in.pointer : m_in.value);

    if (!in) {
        m_out = 1.0f;
        return;
    }
    m_out = 0.0f;
}
