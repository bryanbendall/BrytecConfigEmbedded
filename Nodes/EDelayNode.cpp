#include "EDelayNode.h"

void EDelayNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_in.pointer = output;
        m_mask.setPointer(index);
        break;
    case 1:
        m_delayTime.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void EDelayNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_in.value = value;
        m_mask.setValue(index);
        break;
    case 1:
        m_delayTime.value = value;
        m_mask.setValue(index);
        break;
    case 2:
        m_counter.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* EDelayNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EDelayNode::Evaluate(float timestep)
{
    bool in = ToBool(m_mask.isPointer(0) ? *m_in.pointer : m_in.value);
    float& delayTime = m_mask.isPointer(1) ? *m_delayTime.pointer : m_delayTime.value;
    float& counter = m_mask.isPointer(2) ? *m_counter.pointer : m_counter.value;

    if (in) {
        counter += timestep;
        if (counter >= delayTime) {
            m_out = in;
            counter = delayTime;
        }
    } else {
        counter = 0.0f;
        m_out = 0.0f;
    }
}
