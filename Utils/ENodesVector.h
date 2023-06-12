#pragma once

#include "Boards/BrytecBoard.h"
#include "EBrytecConfig.h"
#include "ENode.h"
#include "Utils/EBrytecErrors.h"
#include <stddef.h>

namespace Brytec {

class ENodesVector {

public:
    ENodesVector() = default;

    ENode* add(const ENodeSpec& spec)
    {
        uint8_t* nextData = &m_nodeData[m_dataNextIndex];

        if ((m_dataNextIndex + MAX_NODE_SIZE) > NODES_VECTOR_SIZE_BYTES) {
            BrytecBoard::error(EBrytecErrors::NodeVectorOutOfSpace);
            return nullptr;
        }

        ENode* node = ENode::CreateInPlace(spec, nextData);

        if (!node) {
            BrytecBoard::error(EBrytecErrors::FailedToCreateNode);
            return nullptr;
        }

        m_count++;
        m_dataNextIndex += node->Size();
        return node;
    }

    ENode* at(uint32_t index)
    {
        if (index >= m_count) {
            BrytecBoard::error(EBrytecErrors::NodeIndexOutOfBounds);
            return nullptr;
        }

        uint32_t nodeDataIndex = 0;
        for (uint32_t indexCount = 0; indexCount < index; indexCount++) {
            ENode* node = (ENode*)&m_nodeData[nodeDataIndex];
            nodeDataIndex += node->Size();
        }

        return (ENode*)&m_nodeData[nodeDataIndex];
    }

    void setValue(uint32_t nodeIndex, uint8_t inputIndex, float value)
    {
        at(nodeIndex)->SetValue(inputIndex, value);
    }

    void setInput(uint32_t nodeIndex, uint8_t inputIndex, uint32_t connectedNodeIndex, uint8_t outputIndex = 0)
    {
        at(nodeIndex)->SetInput(inputIndex, at(connectedNodeIndex)->GetOutput(outputIndex));
    }

    void evaluateAll(float timestep)
    {
        uint32_t nodeDataIndex = 0;
        for (uint32_t indexCount = 0; indexCount < m_count; indexCount++) {
            ENode* node = (ENode*)&m_nodeData[nodeDataIndex];
            node->Evaluate(timestep);
            nodeDataIndex += node->Size();
        }
    }

    void reset()
    {
        m_count = 0;
        m_dataNextIndex = 0;
    }

    uint32_t count() { return m_count; }

    uint32_t getSize() { return m_dataNextIndex; }

private:
    // Data
    uint8_t m_nodeData[NODES_VECTOR_SIZE_BYTES] = {};

    // Vector states
    uint32_t m_count = 0;
    uint32_t m_dataNextIndex = 0;
};

}