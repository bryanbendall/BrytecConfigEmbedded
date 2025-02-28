#pragma once

#include "ENode.h"

#include "Boards/BrytecBoard.h"
#include "Can/EBrytecCan.h"
#include "EBrytecApp.h"

namespace Brytec {

class EHolleyIoModuleNode : public ENode {
public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);

    enum class IoInput : uint8_t {
        Input1 = 0,
        Input2,
        Input3,
        Input4,
        Input5,
        Input6,
        Input7,
        Input8
    };
};

template <typename Data_t>
class EHolleyIoModuleNodeInternal : public EHolleyIoModuleNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
        switch (index) {
        case 0:
            m_data.setPointer(output);
            break;
        }
    }

    void SetValue(uint8_t index, float value) override
    {
        switch (index) {
        case 0:
            m_data.setValue(value);
            break;
        case 1:
            m_canId = FloatToUint(value);
            break;
        case 2:
            m_ioInput = (IoInput)FloatToUint(value);
            break;
        case 3:
            m_sendInterval = value;
            break;
#ifdef NODES_SIMULATION
        case 4:
            m_timerCounter = FloatToUint(value);
            break;
#endif
        }
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 1:
            return (float)m_canId;
        case 2:
            return (float)m_ioInput;
        case 3:
            return m_sendInterval;
        case 4:
            return (float)m_timerCounter;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return nullptr;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        m_timerCounter += timestepMs;
        if (m_timerCounter > FloatToUint(m_sendInterval)) {
            // Send can message
            CanFrame frame;
            frame.type = CanFrameType::Ext;

            // Id
            frame.id = 0x1E202000;
            frame.id |= (m_canId & 0x7FF);
            frame.id |= (((uint8_t)m_ioInput & 0b111) << 14);

            // Data
            float value = m_data;
            uint8_t typeSize = sizeof(float);
            uint8_t* tempValue = (uint8_t*)&value;
            for (uint8_t index = 0; index < typeSize; index++) 
                frame.data[typeSize - 1 - index] = tempValue[index];

            // Get holley can index
            uint8_t canIndex = EBrytecApp::findCanIndex(CanTypes::Types::Holley);
            BrytecBoard::sendCan(canIndex, frame);

            // Reset timer counter
            m_timerCounter = 0;
        }
    }

    uint32_t Size() override
    {
        return sizeof(*this);
    }

    NodeTypes NodeType() override { return NodeTypes::CanBusOutput; }

private:
private:
#if ENODE_FULL_TEMPLATE
    ValueOrPointer<Data_t> m_data;
#else
    ValueAndPointer m_data;
#endif
    uint32_t m_canId;
    IoInput m_ioInput;
    float m_sendInterval;
    uint32_t m_timerCounter;
};
}