#include "EThermistorNode.h"

#include <cmath>

namespace Brytec {

ENode* EThermistorNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Thermistor || spec.numInputs != 1 || spec.numValues != 6)
        return nullptr;

    return new (destination) EThermistorNodeInternal();
}

void EThermistorNodeInternal::SetInput(uint8_t index, float* output)
{
    if (index >= 1)
        return;

    m_input = output;
}

void EThermistorNodeInternal::SetValue(uint8_t index, float value)
{
    switch (index) {
    case 1:
        m_res1 = value;
        break;
    case 2:
        m_temp1 = value;
        break;
    case 3:
        m_res2 = value;
        break;
    case 4:
        m_temp2 = value;
        break;
    case 5:
        m_format = (TempFormat)FloatToUint(value);
        break;
    case 6:
        // Beta calculated
        break;
    }

    CalculateBeta();
}

float EThermistorNodeInternal::GetValue(uint8_t index)
{
    switch (index) {
    case 1:
        return m_res1;
    case 2:
        return m_temp1;
    case 3:
        return m_res2;
    case 4:
        return m_temp2;
    case 5:
        return (float)m_format;
    case 6:
        return m_beta;
    }

    return 0.0f;
}

float* EThermistorNodeInternal::GetOutput(uint8_t index)
{
    return &m_out;
}

void EThermistorNodeInternal::Evaluate(uint32_t timestepMs)
{
    if (!m_input)
        return;

    float tKelvin = (m_beta * ToKelvin(m_temp1)) / (m_beta + (ToKelvin(m_temp1) * log(*m_input / m_res1)));
    m_out = FromKelvin(tKelvin);
}

void EThermistorNodeInternal::CalculateBeta()
{
    if (m_res2 == 0.0f)
        return;

    m_beta = (std::log((m_res1 / m_res2))) / ((1.0f / ToKelvin(m_temp1)) - (1.0f / ToKelvin(m_temp2)));
}

float EThermistorNodeInternal::ToKelvin(float value)
{
    switch (m_format) {
    case Celsius:
        return value + 273.15f;
    case Fahrenheit:
        return (value - 32.0f) * (5.0f / 9.0f) + 273.15f;
    case Kelvin:
        return value;
    }

    return 0.0f;
}

float EThermistorNodeInternal::FromKelvin(float value)
{
    switch (m_format) {
    case Celsius:
        return value - 273.15f;
    case Fahrenheit:
        return (value - 273.15f) * (9.0f / 5.0f) + 32.0f;
    case Kelvin:
        return value;
    }

    return 0.0f;
}
}