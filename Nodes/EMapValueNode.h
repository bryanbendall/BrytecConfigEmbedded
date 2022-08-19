#pragma once

#include "ENode.h"

class EMapValueNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

template <typename Input1_t,
    typename Input2_t,
    typename Input3_t,
    typename Input4_t,
    typename Input5_t>
class EMapValueNodeInternal : public EMapValueNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_in.setPointer(output);
            break;
        case 1:
            m_fromMin.setPointer(output);
            break;
        case 2:
            m_fromMax.setPointer(output);
            break;
        case 3:
            m_toMin.setPointer(output);
            break;
        case 4:
            m_toMax.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_in.setValue(value);
            break;
        case 1:
            m_fromMin.setValue(value);
            break;
        case 2:
            m_fromMax.setValue(value);
            break;
        case 3:
            m_toMin.setValue(value);
            break;
        case 4:
            m_toMax.setValue(value);
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        float x = m_fromMin.getValue();
        float y = m_toMin.getValue();
        float x1 = m_fromMax.getValue();
        float y1 = m_toMax.getValue();
        float sloap = (y - y1) / (x - x1);

        m_out = (sloap * (m_in.getValue() - x1)) + y1;
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Map_Value; }

private:
    ValueOrPointer<Input1_t> m_in;
    ValueOrPointer<Input2_t> m_fromMin;
    ValueOrPointer<Input3_t> m_fromMax;
    ValueOrPointer<Input4_t> m_toMin;
    ValueOrPointer<Input5_t> m_toMax;
    float m_out;
};
