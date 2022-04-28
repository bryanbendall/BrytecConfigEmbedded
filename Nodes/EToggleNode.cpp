#include "EToggleNode.h"

void EToggleNode::SetInput(uint8_t index, float* output)
{
    // switch (index) {
    // case 0:
    //     m_in.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // }
}

void EToggleNode::SetValue(uint8_t index, float value)
{
    // switch (index) {
    // case 0:
    //     m_in.value = value;
    //     m_mask.setValue(index);
    //     break;
    // case 1:
    //     m_lastValue.value = value;
    //     m_mask.setValue(index);
    //     break;
    // }
}

float* EToggleNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void EToggleNode::Evaluate(float timestep)
{
    // bool in = ToBool(m_mask.isPointer(0) ? *m_in.pointer : m_in.value);
    // float& lastValue = m_mask.isPointer(1) ? *m_lastValue.pointer : m_lastValue.value;

    // if (in) {
    //     if (ToBool(lastValue)) {
    //         return;
    //     } else {
    //         lastValue = true;
    //         if (ToBool(m_out))
    //             m_out = 0.0f;
    //         else
    //             m_out = 1.0f;
    //     }
    // } else {
    //     lastValue = false;
    // }
}
