#pragma once

#include "ENode.h"

#include "Boards/BrytecBoard.h"
#include "Can/EBrytecCan.h"

namespace Brytec {

class ECanBusOutputNode : public ENode {
public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    enum class FrameType : uint8_t {
        Std,
        Ext,
    };

    enum class Endian : uint8_t {
        Little,
        Big
    };

    enum class DataType : uint8_t {
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Int8,
        Int16,
        Int32,
        Int64,
        Float
    };
};

template <typename T>
void setData(const float& value, uint8_t* dest, ECanBusOutputNode::Endian endian)
{
    uint8_t typeSize = sizeof(T);
    T intValue = FloatToInt(value);
    uint8_t* tempValue = (uint8_t*)&intValue;
    for (uint8_t index = 0; index < typeSize; index++) {
        if (endian == ECanBusOutputNode::Endian::Little)
            dest[index] = tempValue[index];
        if (endian == ECanBusOutputNode::Endian::Big)
            dest[typeSize - 1 - index] = tempValue[index];
    }
}

template <typename Id_t, typename Data_t, typename Send_t>
class ECanBusOutputNodeInternal : public ECanBusOutputNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_id.setPointer(output);
            break;
        case 1:
            m_data.setPointer(output);
            break;
        case 2:
            m_send.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_id.setValue(value);
            break;
        case 1:
            m_data.setValue(value);
            break;
        case 2:
            m_send.setValue(value);
            break;
        case 3:
            m_canIndex = FloatToInt(value);
            break;
        case 4:
            m_frameType = (FrameType)FloatToInt(value);
            break;
        case 5:
            m_endian = (Endian)FloatToInt(value);
            break;
        case 6:
            m_dataType = (DataType)FloatToInt(value);
            break;
        case 7:
            m_starByte = FloatToInt(value);
            break;
        case 8:
            m_lastSend = value;
            break;
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 3:
            return m_canIndex;
        case 4:
            return (float)m_frameType;
        case 5:
            return (float)m_endian;
        case 6:
            return (float)m_dataType;
        case 7:
            return m_starByte;
        case 8:
            return m_lastSend;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return nullptr;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        bool send = FloatToBool(m_send);

        if (FloatToBool(m_lastSend) != send) {
            m_lastSend = send;
            if (send) {
                CanFrame frame;
                frame.id = FloatToInt(m_id);
                frame.type = m_frameType == FrameType::Ext ? CanFrameType::Ext : CanFrameType::Std;

                switch (m_dataType) {
                case DataType::Uint8:
                    if (m_starByte > 7)
                        return;
                    setData<uint8_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Uint16:
                    if (m_starByte > 6)
                        return;
                    setData<uint16_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Uint64:
                    if (m_starByte > 0)
                        return;
                    setData<uint64_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Uint32:
                    if (m_starByte > 4)
                        return;
                    setData<uint32_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Int8:
                    if (m_starByte > 7)
                        return;
                    setData<int8_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Int16:
                    if (m_starByte > 6)
                        return;
                    setData<int16_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Int32:
                    if (m_starByte > 4)
                        return;
                    setData<int32_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Int64:
                    if (m_starByte > 0)
                        return;
                    setData<int64_t>(m_data, &frame.data[m_starByte], m_endian);
                    break;

                case DataType::Float:
                    if (m_starByte > 4)
                        return;
                    setData<float>(m_data, &frame.data[m_starByte], m_endian);
                    break;
                }

                BrytecBoard::sendCan(FloatToInt(m_canIndex), frame);
            }
        }
    }

    uint32_t Size() override
    {
        return sizeof(*this);
    }

    NodeTypes NodeType() override { return NodeTypes::CanBusOutput; }

private:
private:
#ifdef ENODE_FULL_TEMPLATE
    ValueOrPointer<Id_t> m_id;
    ValueOrPointer<Data_t> m_data;
    ValueOrPointer<Send_t> m_send;
#else
    ValueAndPointer m_id;
    ValueAndPointer m_data;
    ValueAndPointer m_send;
#endif
    uint8_t m_canIndex;
    FrameType m_frameType;
    Endian m_endian;
    DataType m_dataType;
    uint8_t m_starByte;
    float m_lastSend;
};


}