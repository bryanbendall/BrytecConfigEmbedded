#pragma once

#include "ENode.h"

#include "Can/EBrytecCan.h"
#include "EBrytecApp.h"

namespace Brytec {

class EHolleyBroadcastNode : public ENode {

public:
    static ENode* CreateInPlace(const ENodeSpec& spec, uint8_t* destination);
};

class EHolleyBroadcastNodeInternal : public EHolleyBroadcastNode {

public:
    void SetInput(uint8_t index, float* output) override
    {
    }

    void SetValue(uint8_t index, float value) override
    {
        m_channel = FloatToInt(value);
    }

    float GetValue(uint8_t index) override
    {
        switch (index) {
        case 0:
            return m_channel;
        }

        return 0.0f;
    }

    float* GetOutput(uint8_t index = 0) override
    {
        return &m_out;
    }

    void Evaluate(uint32_t timestepMs) override
    {
        if (!EBrytecApp::getHolleyQueue().isDataValid()) {
            m_out = 0.0f;
            return;
        }

        const CanFrame& frame = EBrytecApp::getHolleyQueue().getFrame(m_frameIndex);
        if (!frame)
            return;

        // Convert from big endian to small endian
        uint8_t* v = (uint8_t*)&m_out;
        v[0] = frame.data[3];
        v[1] = frame.data[2];
        v[2] = frame.data[1];
        v[3] = frame.data[0];
    }

    uint32_t Size() override { return sizeof(*this); }

    NodeTypes NodeType() override { return NodeTypes::Holley_Broadcast; }

    void setCanFrameIndex(uint32_t index)
    {
        m_frameIndex = index;
    }

private:
    uint32_t m_channel;
    float m_out = 0.0f;
    uint32_t m_frameIndex = UINT32_MAX;
};
}