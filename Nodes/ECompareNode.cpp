#include "ECompareNode.h"

void ECompareNode::SetInput(uint8_t index, float* output)
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

void ECompareNode::SetValue(uint8_t index, float value)
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
        m_compareType.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* ECompareNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void ECompareNode::Evaluate(float timestep)
{
    float& input1 = m_mask.isPointer(0) ? *m_input1.pointer : m_input1.value;
    float& input2 = m_mask.isPointer(1) ? *m_input2.pointer : m_input2.value;
    Types compareType = (Types)(m_mask.isPointer(2) ? *m_compareType.pointer : m_compareType.value);

    m_out = 0.0f;
    switch (compareType) {
    case Types::Greater:
        if (input1 > input2)
            m_out = 1.0f;
        break;
    case Types::GreaterEqual:
        if (input1 >= input2)
            m_out = 1.0f;
        break;
    case Types::Less:
        if (input1 < input2)
            m_out = 1.0f;
        break;
    case Types::LessEqual:
        if (input1 <= input2)
            m_out = 1.0f;
        break;
    }
}
