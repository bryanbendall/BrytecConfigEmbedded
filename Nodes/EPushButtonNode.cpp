#include "EPushButtonNode.h"

void EPushButtonNode::SetInput(uint8_t index, float* output)
{
    // switch (index) {
    // case 0:
    //     m_button.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // case 1:
    //     m_neutralSafety.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // case 2:
    //     m_engineRunning.pointer = output;
    //     m_mask.setPointer(index);
    //     break;
    // }
}

void EPushButtonNode::SetValue(uint8_t index, float value)
{
    // switch (index) {
    // case 0:
    //     m_button.value = value;
    //     m_mask.setValue(index);
    //     break;
    // case 1:
    //     m_neutralSafety.value = value;
    //     m_mask.setValue(index);
    //     break;
    // case 2:
    //     m_engineRunning.value = value;
    //     m_mask.setValue(index);
    //     break;
    // case 3:
    //     m_lastButtonState.value = value;
    //     m_mask.setValue(index);
    //     break;
    // }
}

float* EPushButtonNode::GetOutput(uint8_t index)
{
    switch (index) {
    case 0:
        return &m_ignitionOut;
    case 1:
        return &m_starterOut;
    }
    return nullptr;
}

void EPushButtonNode::Evaluate(float timestep)
{
    // bool button = ToBool(m_mask.isPointer(0) ? *m_button.pointer : m_button.value);
    // bool neutralSafety = ToBool(m_mask.isPointer(1) ? *m_neutralSafety.pointer : m_neutralSafety.value);
    // bool engineRunning = ToBool(m_mask.isPointer(2) ? *m_engineRunning.pointer : m_engineRunning.value);
    // float& lastButtonState = m_mask.isPointer(3) ? *m_lastButtonState.pointer : m_lastButtonState.value;

    // if (engineRunning && ToBool(m_starterOut)) {
    //     m_starterOut = 0.0f;
    //     return;
    // }

    // if (ToBool(lastButtonState) == button) {
    //     return;
    // }
    // lastButtonState = button;

    // if (button && !ToBool(m_ignitionOut)) {
    //     m_ignitionOut = 1.0f;
    //     m_starterOut = 0.0f;
    //     return;
    // }

    // if (button && !engineRunning && !neutralSafety) {
    //     m_ignitionOut = 0.0f;
    //     m_starterOut = 0.0f;
    //     return;
    // }

    // if (button && engineRunning) {
    //     m_ignitionOut = 0.0f;
    //     m_starterOut = 0.0f;
    //     return;
    // }

    // if (button && !engineRunning && neutralSafety) {
    //     m_starterOut = 1.0f;
    //     return;
    // }

    // m_starterOut = 0.0f;
}
