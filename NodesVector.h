#pragma once

#include "Nodes.h"
#include "Nodes/ENode.h"
#include <stddef.h>

namespace Embedded {

#define SIZE 1000
// template <size_t SIZE>
class NodesVector {

public:
    NodesVector() { }

    bool add(NodeTypes type)
    {
        void* nextData = &m_nodeData[m_dataNextIndex];

        if ((m_dataNextIndex + ENode::getSize(type)) > SIZE)
            return false;

        ENode::create(type, nextData);
        m_size++;
        m_dataNextIndex += ENode::getSize(type);
        return true;
    }

    ENode* at(uint32_t index)
    {
        if (index >= m_size)
            return nullptr;

        uint32_t nodeDataIndex = 0;
        for (uint32_t indexCount = 0; indexCount < index; indexCount++) {
            ENode* node = (ENode*)m_nodeData[indexCount];
            nodeDataIndex += node->Size();
        }

        return (ENode*)m_nodeData[nodeDataIndex];
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
        for (uint32_t indexCount = 0; indexCount < m_size; indexCount++) {
            ENode* node = (ENode*)m_nodeData[indexCount];
            node->Evaluate(timestep);
            nodeDataIndex += node->Size();
        }
    }

private:
    // Data
    uint8_t m_nodeData[SIZE] = {};

    // Vector states
    uint32_t m_size = 0;
    uint32_t m_dataNextIndex = 0;
};

}