#include "ESwitchNode.h"

void ESwitchNode::SetInput(uint8_t index, float* output)
{
    // switch (index) {
    // case 0:
    //     m_selection.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // case 1:
    //     m_input1.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // case 2:
    //     m_input2.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // }
}

void ESwitchNode::SetValue(uint8_t index, float value)
{
    // switch (index) {
    // case 0:
    //     m_selection.value = value;
    //     m_mask.setValue(index);
    //     break;
    // case 1:
    //     m_input1.value = value;
    //     m_mask.setValue(index);
    //     break;
    // case 2:
    //     m_input2.value = value;
    //     m_mask.setValue(index);
    //     break;
    // }
}

float* ESwitchNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void ESwitchNode::Evaluate(float timestep)
{
    // bool selection = ToBool(m_mask.isPointer(0) ? *m_selection.pointer : m_selection.value);
    // float& input1 = m_mask.isPointer(1) ? *m_input1.pointer : m_input1.value;
    // float& input2 = m_mask.isPointer(2) ? *m_input2.pointer : m_input2.value;

    // if (selection)
    //     m_out = input1;
    // else
    //     m_out = input2;
}
