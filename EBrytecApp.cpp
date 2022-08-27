#include "EBrytecApp.h"

#include "Nodes/EFinalValueNode.h"
#include "Nodes/EInitialValueNode.h"
#include "Nodes/ENodeGroupNode.h"
#include "Utils/ENodeDeserializer.h"
#include "Utils/ENodeGroup.h"
#include <stdlib.h>

struct EBrytecAppData {
    uint8_t moduleAddress = 0;
    ENodesVector nodeVector;
    ENodeGroup* nodeGroups;
    uint32_t nodeGroupsCount = 0;
};
static EBrytecAppData s_data;

void EBrytecApp::deserializeModule(BinaryDeserializer& des)
{
    // Delete old node groups because it is a static class
    if (s_data.nodeGroups) {
        s_data.nodeVector.reset();
        free(s_data.nodeGroups);
        s_data.nodeGroups = nullptr;
    }

    // deserialize modules specs
    char m = des.readRaw<char>(); // M
    char d = des.readRaw<char>(); // D
    if (m != 'M' || d != 'D')
        BrytecBoard::error(EBrytecErrors::ModuleHeader);

    uint8_t major = des.readRaw<uint8_t>();
    uint8_t minor = des.readRaw<uint8_t>();
    // TODO check version

    // module info
    des.readRaw<EmptyString>(); // name
    s_data.moduleAddress = des.readRaw<uint8_t>(); // address
    bool moduleEnabled = des.readRaw<uint8_t>(); // enabled
    if (!moduleEnabled) {
        BrytecBoard::error(EBrytecErrors::NotEnabled);
        return;
    }

    // deserialize node groups
    uint16_t nodeGroupCount = des.readRaw<uint16_t>();
    s_data.nodeGroupsCount = nodeGroupCount;

    s_data.nodeGroups = (ENodeGroup*)malloc(sizeof(ENodeGroup) * nodeGroupCount);

    // node group start and length
    for (int nodeGroupIndex = 0; nodeGroupIndex < nodeGroupCount; nodeGroupIndex++) {

        ENodeGroup& currentNodeGroup = s_data.nodeGroups[nodeGroupIndex];
        currentNodeGroup.startNodeIndex = s_data.nodeVector.count();
        currentNodeGroup.boardPinIndex = des.readRaw<uint16_t>();

        // deserialize modules specs
        char n = des.readRaw<char>(); // N
        char g = des.readRaw<char>(); // G
        if (n != 'N' || g != 'G')
            BrytecBoard::error(EBrytecErrors::NodeGroupHeader);

        uint8_t major = des.readRaw<uint8_t>();
        uint8_t minor = des.readRaw<uint8_t>();
        // TODO check version

        // node group info
        des.readRaw<EmptyString>(); // name
        des.readRaw<uint64_t>(); // uuid
        currentNodeGroup.type = (IOTypes::Types)des.readRaw<uint8_t>(); // type
        currentNodeGroup.enabled = des.readRaw<uint8_t>(); // enabled
        if (!currentNodeGroup.enabled)
            BrytecBoard::error(EBrytecErrors::NotEnabled);

        // create nodes in vector
        {
            int nodeCount = des.readRaw<uint16_t>();
            for (int i = 0; i < nodeCount; i++) {
                // Create Node
                ENodeSpec spec = ENodeDeserializer::deserializeNodeSpec(des);
                ENode* node = s_data.nodeVector.add(spec);
                if (!node) {
                    BrytecBoard::error(EBrytecErrors::AddNodeFailed);
                    return;
                }

                // Set Values
                for (int j = 0; j < spec.numValues; j++) {
                    node->SetValue(spec.numInputs + j, spec.values[j]);
                }
            }

            currentNodeGroup.nodeCount = nodeCount;
        }

        // connect nodes
        {
            uint16_t nodeCount = des.readRaw<uint16_t>();
            for (int nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {

                uint8_t inputCount = des.readRaw<uint8_t>();
                for (int inputIndex = 0; inputIndex < inputCount; inputIndex++) {

                    ENodeConnection connection = ENodeDeserializer::deserializeNodeConnection(des);

                    ENode* thisNode = s_data.nodeVector.at(currentNodeGroup.startNodeIndex + nodeIndex);
                    if (connection.connectionNodeIndex != -1 && connection.outputIndex != -1) {
                        // Valid connection
                        ENode* connectedNode = s_data.nodeVector.at(currentNodeGroup.startNodeIndex + connection.connectionNodeIndex);
                        thisNode->SetInput(inputIndex, connectedNode->GetOutput(connection.outputIndex));
                    } else {
                        thisNode->SetValue(inputIndex, connection.defaultValue);
                    }
                }
            }
        }
    }
}

void EBrytecApp::setupModule()
{
    BrytecBoard::setupBrytecCan(s_data.moduleAddress);
}

void EBrytecApp::setupPins()
{
    for (int i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].setupPin();
}

void EBrytecApp::update(float timestep)
{
    // update node group nodes
    updateNodeGroupNodes();

    // get inputs
    for (int i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateInitialValue();
    }

    // calculate all nodes
    s_data.nodeVector.evaluateAll(timestep);

    // set outputs
    for (int i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateFinalValue();
    }
}

ENode* EBrytecApp::getInitialValueNode(int startIndex, int nodeCount)
{
    for (int i = startIndex; i < startIndex + nodeCount; i++) {
        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::Initial_Value)
            return node;
    }

    return nullptr;
}

ENode* EBrytecApp::getFinalValueNode(int startIndex, int nodeCount)
{
    // Start at the last one because it should always be the last
    for (int i = startIndex + nodeCount - 1; i >= startIndex; i--) {
        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::Final_Value)
            return node;
    }

    return nullptr;
}

float EBrytecApp::getBrytecNetworkValue(uint8_t moduleAddress, uint8_t pinIndex)
{
    return BrytecBoard::getBrytecNetworkValue(moduleAddress, pinIndex);
}

ENode* EBrytecApp::getNode(int index)
{
    return s_data.nodeVector.at(index);
}

void EBrytecApp::updateNodeGroupNodes()
{
    for (int i = 0; i < s_data.nodeVector.count(); i++) {

        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNodeInternal* nodeGroupNode = (ENodeGroupNodeInternal*)node;
            float value = EBrytecApp::getBrytecNetworkValue(nodeGroupNode->getModuleAddress(), nodeGroupNode->getPinIndex());
            nodeGroupNode->SetValue(0, value);
        }
    }
}

void EBrytecApp::evaulateJustNodes(float timestep)
{
    // calculate all nodes
    s_data.nodeVector.evaluateAll(timestep);
}