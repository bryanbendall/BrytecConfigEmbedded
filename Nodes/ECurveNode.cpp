#include "ECurveNode.h"

#include <math.h>

void ECurveNode::SetInput(uint8_t index, float* output)
{
    switch (index) {
    case 0:
        m_in.pointer = output;
        m_mask.setPointer(index);
        break;
    case 1:
        m_repeat.pointer = output;
        m_mask.setPointer(index);
        break;
    case 2:
        m_timeout.pointer = output;
        m_mask.setPointer(index);
        break;
    }
}

void ECurveNode::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 0:
        m_in.value = value;
        m_mask.setValue(index);
        break;
    case 1:
        m_repeat.value = value;
        m_mask.setValue(index);
        break;
    case 2:
        m_timeout.value = value;
        m_mask.setValue(index);
        break;
    case 3:
        m_curveType.value = value;
        m_mask.setValue(index);
        break;
    case 4:
        m_timerCounter.value = value;
        m_mask.setValue(index);
        break;
    }
}

float* ECurveNode::GetOutput(uint8_t index)
{
    return index == 0 ? &m_out : nullptr;
}

void ECurveNode::Evaluate(float timestep)
{
    bool in = ToBool(m_mask.isPointer(0) ? *m_in.pointer : m_in.value);
    bool repeat = ToBool(m_mask.isPointer(1) ? *m_repeat.pointer : m_repeat.value);
    float& timeout = m_mask.isPointer(2) ? *m_timeout.pointer : m_timeout.value;
    Types curveType = (Types)(m_mask.isPointer(3) ? *m_curveType.pointer : m_curveType.value);
    float& timerCounter = m_mask.isPointer(4) ? *m_timerCounter.pointer : m_timerCounter.value;

    if (!in) {
        timerCounter = 0.0f;
        m_out = 0.0f;
        return;
    }

    timerCounter += timestep;
    if (timerCounter > timeout) {
        if (repeat)
            timerCounter = 0.0f;
        else
            timerCounter = timeout;
    }
    float curveProgress = timerCounter / timeout;

    switch (curveType) {
    case Types::Toggle:
        if (curveProgress > 0.5f)
            m_out = 1.0f;
        else
            m_out = 0.0f;
        break;
    case Types::Linear:
        m_out = curveProgress;
        break;
    case Types::Exponential:
        m_out = curveProgress * curveProgress;
        break;
    case Types::Breathing:
        float gamma = 0.20f; // affects the width of peak (more or less darkness)
        float beta = 0.5f;
        m_out = (expf(-(powf((curveProgress - beta) / gamma, 2.0f)) / 2.0f)); //(exp ^ exp) / 2.0f;
        break;
    }
}
