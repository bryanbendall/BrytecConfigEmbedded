#include "EAndNode.h"

void EAndNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_in1.pointer = output;
        m_pinMask.setPointer(index);
        break;
    case 1:
        m_in2.pointer = output;
        m_pinMask.setPointer(index);
        break;
    case 2:
        m_in3.pointer = output;
        m_pinMask.setPointer(index);
        break;
    case 3:
        m_in4.pointer = output;
        m_pinMask.setPointer(index);
        break;
    case 4:
        m_in5.pointer = output;
        m_pinMask.setPointer(index);
        break;
    }
}

void EAndNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_in1.value = value;
        m_pinMask.setValue(index);
        break;
    case 1:
        m_in2.value = value;
        m_pinMask.setValue(index);
        break;
    case 2:
        m_in3.value = value;
        m_pinMask.setValue(index);
        break;
    case 3:
        m_in4.value = value;
        m_pinMask.setValue(index);
        break;
    case 4:
        m_in5.value = value;
        m_pinMask.setValue(index);
        break;
    }
}

float* EAndNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EAndNode::Evaluate(float timestep)
{
    bool in1 = ToBool(m_pinMask.isPointer(0) ? *m_in1.pointer : m_in1.value);
    bool in2 = ToBool(m_pinMask.isPointer(1) ? *m_in2.pointer : m_in2.value);
    bool in3 = ToBool(m_pinMask.isPointer(2) ? *m_in3.pointer : m_in3.value);
    bool in4 = ToBool(m_pinMask.isPointer(3) ? *m_in4.pointer : m_in4.value);
    bool in5 = ToBool(m_pinMask.isPointer(4) ? *m_in5.pointer : m_in5.value);

    m_out = 0.0f;

    if (!in1)
        return;
    if (!in2)
        return;
    if (!in3)
        return;
    if (!in4)
        return;
    if (!in5)
        return;

    m_out = 1.0f;
}
