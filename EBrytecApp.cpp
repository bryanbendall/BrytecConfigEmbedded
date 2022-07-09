#include "EBrytecApp.h"

void EBrytecApp::deserializeModule()
{

    // create nodes in vector

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