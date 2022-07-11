#include "EBrytecApp.h"

#include "Utils/BinaryDeserializer.h"
#include "Utils/ENodeDeserializer.h"

void EBrytecApp::deserializeModule()
{

    BinaryDeserializer des(nullptr);

    // create nodes in vector
    int nodeCount = 0;
    for (int i = 0; i < nodeCount; i++) {
        ENodeSpec spec = ENodeDeserializer::deserializeNode(des);
        m_nodeVector.add(spec);
    }

    // connect nodes

    // allocate nodegroups array and assign node start and count
}

void EBrytecApp::update(float timestep)
{
    // get inputs
    for (int i = 0; i < m_nodeGroupsCount; i++) {
        m_nodeGroups[i].updateInitialValue();
    }

    // calculate all nodes
    m_nodeVector.evaluateAll(timestep);

    // set outputs
    for (int i = 0; i < m_nodeGroupsCount; i++) {
        m_nodeGroups[i].updateFinalValue();
    }
}