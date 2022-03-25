#include "EMathNode.h"

void EMathNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_input1.pointer = output;
        m_mask.setPointer(index);
        break;
    case 1:
        m_input2.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void EMathNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_input1.value = value;
        m_mask.setValue(index);
        break;
    case 1:
        m_input2.value = value;
        m_mask.setValue(index);
        break;
    case 2:
        m_mathType.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* EMathNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EMathNode::Evaluate(float timestep)
{
    float& input1 = m_mask.isPointer(0) ? *m_input1.pointer : m_input1.value;
    float& input2 = m_mask.isPointer(1) ? *m_input2.pointer : m_input2.value;
    Types mathType = (Types)(m_mask.isPointer(2) ? *m_mathType.pointer : m_mathType.value);

    switch (mathType) {
    case Types::Add:
        m_out = input1 + input2;
        break;
    case Types::Subtract:
        m_out = input1 - input2;
        break;
    case Types::Multiply:
        m_out = input1 * input2;
        break;
    case Types::Divide:
        m_out = input1 / input2;
        break;
    }
}
