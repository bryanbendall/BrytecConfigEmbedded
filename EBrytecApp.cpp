#include "EBrytecApp.h"

#include "Can/EPinStatusQueue.h"
#include "Nodes/EFinalValueNode.h"
#include "Nodes/EInitialValueNode.h"
#include "Nodes/ENodeGroupNode.h"
#include "Utils/ENodeDeserializer.h"
#include "Utils/ENodeGroup.h"
#include <stdlib.h>

struct EBrytecAppData {
    uint8_t moduleAddress = 0;
    ENodesVector nodeVector = {};
    ENodeGroup* nodeGroups = nullptr;
    uint16_t nodeGroupsCount = 0;
    EPinStatusQueue statusQueue;
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
    char m, d;
    des.readRaw<char>(&m); // M
    des.readRaw<char>(&d); // D

    if (m != 'M' || d != 'D')
        BrytecBoard::error(EBrytecErrors::ModuleHeader);

    uint8_t major, minor;
    des.readRaw<uint8_t>(&major);
    des.readRaw<uint8_t>(&minor);
    // TODO check version

    // Module name
    EmptyString empty;
    des.readRaw<EmptyString>(&empty);

    // Module address
    des.readRaw<uint8_t>(&s_data.moduleAddress);

    // Module enabled
    uint8_t moduleEnabled;
    des.readRaw<uint8_t>(&moduleEnabled);
    if (!moduleEnabled) {
        BrytecBoard::error(EBrytecErrors::NotEnabled);
        return;
    }

    // Deserialize node groups
    // Total Node Groups
    des.readRaw<uint16_t>(&s_data.nodeGroupsCount);

    // Allocate space for node groups
    s_data.nodeGroups = (ENodeGroup*)malloc(sizeof(ENodeGroup) * s_data.nodeGroupsCount);
    if (!s_data.nodeGroups)
        BrytecBoard::error(EBrytecErrors::BadAlloc);

    // Physical Node Group Count
    uint16_t physicalNodeGroupCount;
    des.readRaw<uint16_t>(&physicalNodeGroupCount);

    // Node Group Loop
    for (uint16_t nodeGroupIndex = 0; nodeGroupIndex < s_data.nodeGroupsCount; nodeGroupIndex++) {

        // Internal Node Group Count
        if (nodeGroupIndex == physicalNodeGroupCount) {
            uint16_t internalNodeGroupCount;
            des.readRaw<uint16_t>(&internalNodeGroupCount);
        }

        ENodeGroup& currentNodeGroup = s_data.nodeGroups[nodeGroupIndex];
        currentNodeGroup.startNodeIndex = s_data.nodeVector.count();
        des.readRaw<uint16_t>(&currentNodeGroup.boardPinIndex);

        // Deserialize modules specs
        char n, g;
        des.readRaw<char>(&n); // N
        des.readRaw<char>(&g); // G
        if (n != 'N' || g != 'G')
            BrytecBoard::error(EBrytecErrors::NodeGroupHeader);

        uint8_t nodeGroupMajor, nodeGroupMinor;
        des.readRaw<uint8_t>(&nodeGroupMajor);
        des.readRaw<uint8_t>(&nodeGroupMinor);
        // TODO check version

        // Node Group name
        EmptyString empty;
        des.readRaw<EmptyString>(&empty);

        // Node Group uuid
        uint64_t uuid;
        des.readRaw<uint64_t>(&uuid);

        // Node Group type
        uint8_t type;
        des.readRaw<uint8_t>(&type);
        currentNodeGroup.type = (IOTypes::Types)type;

        // Node Group enabled
        uint8_t enabled;
        des.readRaw<uint8_t>(&enabled);
        currentNodeGroup.enabled = enabled;
        if (!currentNodeGroup.enabled)
            BrytecBoard::error(EBrytecErrors::NotEnabled);

        // Create nodes in vector
        {
            uint16_t nodeCount;
            des.readRaw<uint16_t>(&nodeCount);

            for (uint16_t i = 0; i < nodeCount; i++) {

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
            uint16_t nodeCount;
            des.readRaw<uint16_t>(&nodeCount);

            for (uint16_t nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {

                uint8_t inputCount;
                des.readRaw<uint8_t>(&inputCount);
                for (uint8_t inputIndex = 0; inputIndex < inputCount; inputIndex++) {

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
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].setupPin();
}

void EBrytecApp::update(float timestep)
{
    // Update node group nodes
    updateNodeGroupNodes();

    // Get inputs
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateInitialValue();
    }

    // Calculate all nodes
    s_data.nodeVector.evaluateAll(timestep);

    // Set outputs
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateFinalValue();
    }

    // Send can data
    static float canDataTimer = 0.0f;
    canDataTimer += timestep;
    if (canDataTimer >= CAN_UPDATE_FREQUENCY) {
        sendBrytecCanData();
        canDataTimer = 0.0f;
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

ENode* EBrytecApp::getPinCurrentNode(int startIndex, int nodeCount)
{
    for (int i = startIndex; i < startIndex + nodeCount; i++) {
        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::PinCurrent)
            return node;
    }

    return nullptr;
}

ENode* EBrytecApp::getNode(int index)
{
    return s_data.nodeVector.at(index);
}

void EBrytecApp::queueCanData(const EBrytecCan::PinStatusBroadcast& bc)
{
    s_data.statusQueue.add(bc);
}

void EBrytecApp::sendBrytecCanData()
{
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {

        ENodeGroup& nodeGroup = s_data.nodeGroups[i];

        EBrytecCan::PinStatusBroadcast bc;
        bc.moduleAddress = s_data.moduleAddress;
        bc.nodeGroupIndex = nodeGroup.boardPinIndex;
        if (nodeGroup.enabled)
            bc.statusFlags = EBrytecCan::PinStatusBroadcast::StatusFlags::NORMAL;
        bc.value = nodeGroup.getFinalValue();
        bc.voltage = BrytecBoard::getPinVoltage(nodeGroup.boardPinIndex);
        bc.current = BrytecBoard::getPinCurrent(nodeGroup.boardPinIndex);

        BrytecBoard::sendBrytecCan(bc.getFrame());
    }
}

ENodeGroupNodeInternal* EBrytecApp::findNodeGroupNode(uint8_t moduleAddress, uint16_t nodeGroupIndex)
{
    for (uint32_t i = 0; i < s_data.nodeVector.count(); i++) {

        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNodeInternal* nodeGroupNode = (ENodeGroupNodeInternal*)node;
            if (nodeGroupNode->getModuleAddress() == moduleAddress && nodeGroupNode->getNodeGroupIndex() == nodeGroupIndex)
                return nodeGroupNode;
        }
    }

    return nullptr;
}

void EBrytecApp::updateNodeGroupNodes()
{
    // Internal nodes to this module
    {
        for (unsigned int i = 0; i < s_data.nodeVector.count(); i++) {
            ENode* node = s_data.nodeVector.at(i);
            if (node->NodeType() == NodeTypes::Node_Group) {
                ENodeGroupNodeInternal* nodeGroupNode = (ENodeGroupNodeInternal*)node;
                if (nodeGroupNode->getModuleAddress() == s_data.moduleAddress) {

                    // In this module
                    for (uint16_t j = 0; j < s_data.nodeGroupsCount; j++) {
                        if (s_data.nodeGroups[j].boardPinIndex == nodeGroupNode->getNodeGroupIndex())
                            nodeGroupNode->SetValue(0, s_data.nodeGroups[j].getFinalValue());
                    }
                }
            }
        }
    }

    // External nodes from can messages
    {
        for (int i = 0; i < s_data.statusQueue.size(); i++) {
            EBrytecCan::PinStatusBroadcast* bc = s_data.statusQueue.at(i);
            if (!bc)
                continue;

            ENodeGroupNodeInternal* nodeGroupNode = findNodeGroupNode(bc->moduleAddress, bc->nodeGroupIndex);
            if (nodeGroupNode)
                nodeGroupNode->SetValue(0, bc->value);
        }

        s_data.statusQueue.clear();
    }
}

void EBrytecApp::evaulateJustNodes(float timestep)
{
    // Calculate all nodes
    s_data.nodeVector.evaluateAll(timestep);
}