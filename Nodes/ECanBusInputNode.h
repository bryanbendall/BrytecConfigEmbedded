#pragma once

#include "ENode.h"

#include "Can/EBrytecCan.h"
#include "EBrytecApp.h"

namespace Brytec {

class ECanBusInputNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    enum class Endian : uint8_t {
        Little,
        Big
    };

    enum class DataType : uint8_t {
        Uint8,
        Uint16,
        Uint32,
        Int8,
        Int16,
        Int32,
        Float
    };
};

class ECanBusInputNodeInternal : public ECanBusInputNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            // Store as a uint32_t
            memcpy(&m_id, &value, sizeof(m_id));
            break;
        case 1:
            m_canIndex = FloatToUint(value);
            break;
        case 2:
            m_endian = (Endian)FloatToUint(value);
            break;
        case 3:
            m_dataType = (DataType)FloatToUint(value);
            break;
        case 4:
            m_starByte = FloatToUint(value);
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 0: {
            // Return as uint32_t
            float ret;
            memcpy(&ret, &m_id, sizeof(ret));
            return ret;
        }
        case 1:
            return m_canIndex;
        case 2:
            return (float)m_endian;
        case 3:
            return (float)m_dataType;
        case 4:
            return m_starByte;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        CanFrame frame = EBrytecApp::getCustomCanFrame(m_canIndex, m_frameIndex);
        if (!frame)
            return;

        // TODO check for timeout?

        switch (m_dataType) {
        case DataType::Uint8:
            if (m_starByte > 7)
                return;
            m_out = getFloat<uint8_t>(&frame.data[m_starByte], m_endian);
            break;

        case DataType::Uint16:
            if (m_starByte > 6)
                return;
            m_out = getFloat<uint16_t>(&frame.data[m_starByte], m_endian);
            break;

        case DataType::Uint32:
            if (m_starByte > 4)
                return;
            m_out = getFloat<uint32_t>(&frame.data[m_starByte], m_endian);
            break;

        case DataType::Int8:
            if (m_starByte > 7)
                return;
            m_out = getFloat<int8_t>(&frame.data[m_starByte], m_endian);
            break;

        case DataType::Int16:
            if (m_starByte > 6)
                return;
            m_out = getFloat<int16_t>(&frame.data[m_starByte], m_endian);
            break;

        case DataType::Int32:
            if (m_starByte > 4)
                return;
            m_out = getFloat<int32_t>(&frame.data[m_starByte], m_endian);
            break;

        case DataType::Float:
            if (m_starByte > 4)
                return;
            m_out = getFloat<float>(&frame.data[m_starByte], m_endian);
            break;
        }
    }

    uint32_t Size() override
    {
        return sizeof(*this);
    }

    NodeTypes NodeType() override { return NodeTypes::CanBusInput; }

    void setCanFrameIndex(uint32_t index)
    {
        m_frameIndex = index;
    }

private:
    template <typename T>
    float getFloat(uint8_t* data, Endian endian)
    {
        uint8_t typeSize = sizeof(T);
        T value;
        uint8_t* tempValue = (uint8_t*)&value;
        for (uint8_t index = 0; index < typeSize; index++) {
            if (endian == Endian::Little)
                tempValue[index] = data[index];
            if (endian == Endian::Big)
                tempValue[index] = data[typeSize - 1 - index];
        }
        return value;
    }

private:
    uint32_t m_id;
    uint8_t m_canIndex;
    Endian m_endian;
    DataType m_dataType;
    uint8_t m_starByte;
    float m_out = 0.0f;
    uint32_t m_frameIndex = UINT32_MAX;
};
}