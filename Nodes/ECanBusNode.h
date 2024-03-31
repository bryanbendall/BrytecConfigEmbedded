#pragma once

#include "ENode.h"

#include "Can/EBrytecCan.h"

namespace Brytec {

class ECanBusNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class ECanBusNodeInternal : public ECanBusNode {

    enum Endian : uint8_t {
        Little,
        Big
    };

    enum DataType : uint8_t {
        Uint8,
        Uint16,
        Uint32,
        Int8,
        Int16,
        Int32,
        Float
    };

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_id = FloatToInt(value);
            break;
        case 1:
            m_canIndex = FloatToInt(value);
            break;
        case 2:
            m_endian = (Endian)FloatToInt(value);
            break;
        case 3:
            m_dataType = (DataType)FloatToInt(value);
            break;
        case 4:
            m_starByte = FloatToInt(value);
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 0:
            return m_id;
        case 1:
            return m_canIndex;
        case 2:
            return m_endian;
        case 3:
            return m_dataType;
        case 4:
            return m_starByte;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(float timestep) override
    {
        switch (m_dataType) {
        case Uint8:
            if (m_starByte > 7)
                return;
            m_out = getFloat<uint8_t>(&m_frame.data[m_starByte], m_endian);
            break;

        case Uint16:
            if (m_starByte > 6)
                return;
            m_out = getFloat<uint16_t>(&m_frame.data[m_starByte], m_endian);
            break;

        case Uint32:
            if (m_starByte > 4)
                return;
            m_out = getFloat<uint32_t>(&m_frame.data[m_starByte], m_endian);
            break;

        case Int8:
            if (m_starByte > 7)
                return;
            m_out = getFloat<int8_t>(&m_frame.data[m_starByte], m_endian);
            break;

        case Int16:
            if (m_starByte > 6)
                return;
            m_out = getFloat<int16_t>(&m_frame.data[m_starByte], m_endian);
            break;

        case Int32:
            if (m_starByte > 4)
                return;
            m_out = getFloat<int32_t>(&m_frame.data[m_starByte], m_endian);
            break;

        case Float:
            if (m_starByte > 4)
                return;
            m_out = getFloat<float>(&m_frame.data[m_starByte], m_endian);
            break;
        }
    }

    uint32_t Size() override
    {
        return sizeof(*this);
    }

    NodeTypes NodeType() override { return NodeTypes::CanBus; }

    void setCanFrame(CanFrame frame)
    {
        m_frame = frame;
    }

private:
    template <typename T>
    float getFloat(uint8_t* data, Endian endian)
    {
        uint8_t typeSize = sizeof(T);
        float finalValue = 0.0f;
        uint8_t* tempValue = (uint8_t*)&finalValue;
        for (uint8_t index = 0; index < typeSize; index++) {
            if (endian == Endian::Little)
                tempValue[index] = data[index];
            if (endian == Endian::Big)
                tempValue[index] = data[typeSize - index];
        }
        return finalValue;
    }

private:
    uint32_t m_id;
    uint8_t m_canIndex;
    Endian m_endian;
    DataType m_dataType;
    uint8_t m_starByte;
    float m_out;
    CanFrame m_frame;
};
}