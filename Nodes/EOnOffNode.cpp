#include "EOnOffNode.h"

void EOnOffNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_on.pointer = output;
        m_mask.setPointer(index);
        break;
    case 1:
        m_off.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void EOnOffNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_on.value = value;
        m_mask.setValue(index);
        break;
    case 1:
        m_off.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* EOnOffNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EOnOffNode::Evaluate(float timestep)
{
    bool on = ToBool(m_mask.isPointer(0) ? *m_on.pointer : m_on.value);
    bool off = ToBool(m_mask.isPointer(1) ? *m_off.pointer : m_off.value);

    if (off) {
        m_out = 0.0f;
        return;
    } else if (on) {
        m_out = 1.0f;
        return;
    }
    m_out = 0.0f;
}
