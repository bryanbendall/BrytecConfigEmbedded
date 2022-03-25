#include "EMapValueNode.h"

void EMapValueNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_in.pointer = output;
        m_mask.setPointer(index);
        break;
    case 1:
        m_fromMin.pointer = output;
        m_mask.setPointer(index);
        break;
    case 2:
        m_fromMax.pointer = output;
        m_mask.setPointer(index);
        break;
    case 3:
        m_toMin.pointer = output;
        m_mask.setPointer(index);
        break;
    case 4:
        m_toMax.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void EMapValueNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_in.value = value;
        m_mask.setValue(index);
        break;
    case 1:
        m_fromMin.value = value;
        m_mask.setValue(index);
        break;
    case 2:
        m_fromMax.value = value;
        m_mask.setValue(index);
        break;
    case 3:
        m_toMin.value = value;
        m_mask.setValue(index);
        break;
    case 4:
        m_toMax.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* EMapValueNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EMapValueNode::Evaluate(float timestep)
{
    float& in = m_mask.isPointer(0) ? *m_in.pointer : m_in.value;
    float& fromMin = m_mask.isPointer(1) ? *m_fromMin.pointer : m_fromMin.value;
    float& fromMax = m_mask.isPointer(2) ? *m_fromMax.pointer : m_fromMax.value;
    float& toMin = m_mask.isPointer(3) ? *m_toMin.pointer : m_toMin.value;
    float& toMax = m_mask.isPointer(4) ? *m_toMax.pointer : m_toMax.value;

    float x = fromMin;
    float y = toMin;
    float x1 = fromMax;
    float y1 = toMax;
    float sloap = (y - y1) / (x - x1);

    m_out = (sloap * (in - x1)) + y1;
}
