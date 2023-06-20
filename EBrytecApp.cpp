#include "EBrytecApp.h"

#include "Can/ECanCommandQueue.h"
#include "Can/EPinStatusQueue.h"
#include "Deserializer/BinaryArrayDeserializer.h"
#include "Deserializer/BinaryBufferSerializer.h"
#include "Nodes/EFinalValueNode.h"
#include "Nodes/EInitialValueNode.h"
#include "Nodes/ENodeGroupNode.h"
#include "Utils/ENodeDeserializer.h"
#include "Utils/ENodeGroup.h"
#include <stdlib.h>

namespace Brytec {

struct EBrytecAppData {
    EBrytecApp::Mode mode = EBrytecApp::Mode::Stopped;
    bool deserializeOk = false;
    uint8_t moduleAddress = 254;
    ENodesVector nodeVector = {};
    ENodeGroup* nodeGroups = nullptr;
    uint16_t nodeGroupsCount = 0;
    EPinStatusQueue statusQueue;
    ECanCommandQueue canCommandQueue;
};
static EBrytecAppData s_data;

void EBrytecApp::initalize()
{
    deserializeModule();
    if (s_data.deserializeOk)
        setMode(Mode::Normal);
    else
        BrytecBoard::setupBrytecCan(0, 0);
    // Setup can with default address so it can be programmed
}

void EBrytecApp::deserializeModule()
{
    s_data.deserializeOk = false;

    BinaryDeserializer* des = BrytecBoard::getDeserializer();
    if (!des)
        return;

    // Delete old node groups because it is a static class
    if (s_data.nodeGroups) {
        s_data.nodeVector.reset();
        free(s_data.nodeGroups);
        s_data.nodeGroups = nullptr;
    }

    // deserialize modules specs
    char m, d;
    des->readRaw<char>(&m); // M
    des->readRaw<char>(&d); // D

    if (m != 'M' || d != 'D') {
        BrytecBoard::error(EBrytecErrors::ModuleHeader);
        return;
    }

    uint8_t major, minor;
    des->readRaw<uint8_t>(&major);
    des->readRaw<uint8_t>(&minor);
    // TODO check version

    // Module name
    EmptyString empty;
    des->readRaw<EmptyString>(&empty);

    // Manufacturer name
    des->readRaw<EmptyString>(&empty);

    // Board name
    des->readRaw<EmptyString>(&empty);

    // Module address
    des->readRaw<uint8_t>(&s_data.moduleAddress);

    // Module enabled
    uint8_t moduleEnabled;
    des->readRaw<uint8_t>(&moduleEnabled);
    if (!moduleEnabled) {
        BrytecBoard::error(EBrytecErrors::ModuleNotEnabled);
        return;
    }

    // Deserialize node groups
    // Total Node Groups
    des->readRaw<uint16_t>(&s_data.nodeGroupsCount);

    // Allocate space for node groups
    s_data.nodeGroups = (ENodeGroup*)malloc(sizeof(ENodeGroup) * s_data.nodeGroupsCount);
    if (!s_data.nodeGroups) {
        BrytecBoard::error(EBrytecErrors::BadAlloc);
        return;
    }

    // Physical Node Group Count
    uint16_t physicalNodeGroupCount;
    des->readRaw<uint16_t>(&physicalNodeGroupCount);

    // Node Group Loop
    for (uint16_t nodeGroupIndex = 0; nodeGroupIndex < s_data.nodeGroupsCount; nodeGroupIndex++) {

        // Internal Node Group Count
        if (nodeGroupIndex == physicalNodeGroupCount) {
            uint16_t internalNodeGroupCount;
            des->readRaw<uint16_t>(&internalNodeGroupCount);
        }

        ENodeGroup* currentNodeGroup = new (&s_data.nodeGroups[nodeGroupIndex]) ENodeGroup();
        currentNodeGroup->startNodeIndex = s_data.nodeVector.count();
        des->readRaw<uint16_t>(&currentNodeGroup->index);

        // Deserialize node group specs
        char n, g;
        des->readRaw<char>(&n); // N
        des->readRaw<char>(&g); // G
        if (n != 'N' || g != 'G') {
            BrytecBoard::error(EBrytecErrors::NodeGroupHeader);
            return;
        }

        // Version
        uint8_t nodeGroupMajor, nodeGroupMinor;
        des->readRaw<uint8_t>(&nodeGroupMajor);
        des->readRaw<uint8_t>(&nodeGroupMinor);
        // TODO check version

        // Node Group name
        EmptyString empty;
        des->readRaw<EmptyString>(&empty);

        // Node Group uuid
        uint64_t uuid;
        des->readRaw<uint64_t>(&uuid);

        // Node Group type
        uint8_t type;
        des->readRaw<uint8_t>(&type);
        currentNodeGroup->type = (IOTypes::Types)type;

        // Node Group enabled
        uint8_t enabled;
        des->readRaw<uint8_t>(&enabled);
        currentNodeGroup->enabled = enabled;

        // Node Group used on bus
        uint8_t usedOnBus;
        des->readRaw<uint8_t>(&usedOnBus);
        currentNodeGroup->usedOnBus = usedOnBus;

        // Current limit
        des->readRaw<uint8_t>(&currentNodeGroup->currentLimit);

        uint8_t alwaysRetry;
        des->readRaw<uint8_t>(&alwaysRetry);
        currentNodeGroup->alwaysRetry = alwaysRetry;

        des->readRaw<uint8_t>(&currentNodeGroup->maxRetries);

        des->readRaw<float>(&currentNodeGroup->retryDelay);

        // Create nodes in vector
        {
            uint16_t nodeCount;
            des->readRaw<uint16_t>(&nodeCount);

            for (uint16_t i = 0; i < nodeCount; i++) {

                // Create Node
                ENodeSpec spec = ENodeDeserializer::deserializeNodeSpec(*des);

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

            currentNodeGroup->nodeCount = nodeCount;
        }

        // connect nodes
        {
            uint16_t nodeCount;
            des->readRaw<uint16_t>(&nodeCount);

            for (uint16_t nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {

                ENode* thisNode = s_data.nodeVector.at(currentNodeGroup->startNodeIndex + nodeIndex);

                uint8_t inputCount;
                des->readRaw<uint8_t>(&inputCount);
                for (uint8_t inputIndex = 0; inputIndex < inputCount; inputIndex++) {

                    ENodeConnection connection = ENodeDeserializer::deserializeNodeConnection(*des);

                    if (connection.connectionNodeIndex != -1 && connection.outputIndex != -1) {
                        // Valid connection
                        ENode* connectedNode = s_data.nodeVector.at(currentNodeGroup->startNodeIndex + connection.connectionNodeIndex);
                        thisNode->SetInput(inputIndex, connectedNode->GetOutput(connection.outputIndex));
                    } else {
                        thisNode->SetValue(inputIndex, connection.defaultValue);
                    }
                }
            }
        }
    }

    s_data.deserializeOk = true;
}

bool EBrytecApp::isDeserializeOk()
{
    return s_data.deserializeOk;
}

void EBrytecApp::setupModule()
{
    if (!s_data.deserializeOk)
        return;

    // Mask and filter for node group nodes
    uint32_t mask = 0;
    uint32_t filter = 0;
    calculateMaskAndFilter(&mask, &filter);

    BrytecBoard::setupBrytecCan(mask, filter);
}

void EBrytecApp::setupPins()
{
    if (!s_data.deserializeOk)
        return;

    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].setupPin();
}

void EBrytecApp::update(float timestep)
{
    processCanCommands();

    // Only update in normal mode
    if (s_data.mode != EBrytecApp::Mode::Normal)
        return;

    if (!s_data.deserializeOk)
        return;

    // Update current and over current
    updateCurrents(timestep);

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
        sendBrytecCanBroadcasts();
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

void EBrytecApp::brytecCanReceived(const CanExtFrame& frame)
{
    if (frame.isBroadcast()) {
        if (s_data.mode != Mode::Normal)
            return;
        PinStatusBroadcast pinStatus(frame);
        s_data.statusQueue.add(pinStatus);
    } else {
        CanCommands canCommand(frame);
        s_data.canCommandQueue.add(canCommand);
    }
}

void EBrytecApp::sendBrytecCanBroadcasts()
{
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {

        ENodeGroup& nodeGroup = s_data.nodeGroups[i];
        if (!nodeGroup.enabled || !nodeGroup.usedOnBus)
            continue;

        sendBrytecCanPinStatus(nodeGroup);
    }
}

void EBrytecApp::sendBrytecCanPinStatus(ENodeGroup& nodeGroup)
{
    PinStatusBroadcast pinStatus;
    pinStatus.moduleAddress = s_data.moduleAddress;
    pinStatus.nodeGroupIndex = nodeGroup.index;
    if (nodeGroup.enabled)
        pinStatus.statusFlags = PinStatusBroadcast::StatusFlags::NORMAL;
    if (nodeGroup.tripped)
        pinStatus.statusFlags = PinStatusBroadcast::StatusFlags::TRIPPED;
    pinStatus.value = nodeGroup.getFinalValue();
    pinStatus.voltage = BrytecBoard::getPinVoltage(nodeGroup.index);
    pinStatus.current = BrytecBoard::getPinCurrent(nodeGroup.index);

    BrytecBoard::sendBrytecCan(pinStatus.getFrame());
}

ENodeGroupNode* EBrytecApp::findNodeGroupNode(uint8_t moduleAddress, uint16_t nodeGroupIndex)
{
    for (uint32_t i = 0; i < s_data.nodeVector.count(); i++) {

        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)node;
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
                ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)node;
                if (nodeGroupNode->getModuleAddress() == s_data.moduleAddress) {

                    // In this module
                    for (uint16_t j = 0; j < s_data.nodeGroupsCount; j++) {
                        if (s_data.nodeGroups[j].index == nodeGroupNode->getNodeGroupIndex())
                            nodeGroupNode->SetValue(99, s_data.nodeGroups[j].getFinalValue());
                    }
                }
            }
        }
    }

    // External nodes from can messages
    {
        // Copy queue in case we get new messages while we are updating
        EPinStatusQueue tempQueue = s_data.statusQueue;
        s_data.statusQueue.clear();

        for (int i = 0; i < tempQueue.size(); i++) {
            PinStatusBroadcast* pinStatus = tempQueue.at(i);
            if (!pinStatus)
                continue;

            // We need to go through all nodes beacuse we might match more then one
            for (uint32_t i = 0; i < s_data.nodeVector.count(); i++) {
                ENode* node = s_data.nodeVector.at(i);
                if (node->NodeType() == NodeTypes::Node_Group) {
                    ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)node;
                    if (nodeGroupNode->getModuleAddress() == pinStatus->moduleAddress && nodeGroupNode->getNodeGroupIndex() == pinStatus->nodeGroupIndex)
                        nodeGroupNode->SetValue(99, pinStatus->value);
                }
            }
        }
    }
}

void EBrytecApp::updateCurrents(float timestep)
{
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].updatePinCurrent(timestep);
}

void EBrytecApp::evaulateJustNodes(float timestep)
{
    // Calculate all nodes
    s_data.nodeVector.evaluateAll(timestep);
}

void EBrytecApp::calculateMaskAndFilter(uint32_t* mask, uint32_t* filter)
{
    // Start mask all enabled except status flags
    *mask = 0b10000111111111111111111111111;

    // Check all node group nodes
    for (uint32_t i = 0; i < s_data.nodeVector.count(); i++) {
        ENode* node = s_data.nodeVector.at(i);
        if (node->NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)node;

            // Skip if it is this module
            if (nodeGroupNode->getModuleAddress() == s_data.moduleAddress)
                continue;

            // Get can frame to get id
            PinStatusBroadcast bc;
            bc.moduleAddress = nodeGroupNode->getModuleAddress();
            bc.nodeGroupIndex = nodeGroupNode->getNodeGroupIndex();
            CanExtFrame frame = bc.getFrame();

            if (*filter == 0) {
                *filter = frame.id;
            } else {
                // calculate mask
                uint32_t sameBits = (*filter & frame.id) | (~(*filter) & ~frame.id);
                (*mask) &= sameBits;
            }
        }
    }
}

void EBrytecApp::processCanCommands()
{
    if (s_data.canCommandQueue.size() == 0)
        return;

    // Copy queue in case we get new messages while we are updating
    ECanCommandQueue tempQueue = s_data.canCommandQueue;
    s_data.canCommandQueue.clear();

    for (uint8_t i = 0; i < tempQueue.size(); i++) {
        CanCommands* canCommand = tempQueue.at(i);

        // Not for this module or all modules
        if (canCommand->moduleAddress != s_data.moduleAddress && canCommand->moduleAddress != CanCommands::AllModules)
            continue;

        switch (canCommand->command) {
        case CanCommands::Command::ChangeMode:
            setMode((EBrytecApp::Mode)canCommand->data[0]);
            sendCanAck();
            sendCanModuleStatus();
            break;
        case CanCommands::Command::ChangeAddress:
            if (s_data.mode != EBrytecApp::Mode::Stopped)
                sendCanNak();
            else {
                sendCanAck();
                s_data.moduleAddress = canCommand->data[0];
                sendCanModuleStatus();
            }
            break;
        case CanCommands::Command::ReloadConfig:
            if (s_data.mode != EBrytecApp::Mode::Stopped)
                sendCanNak();
            else {
                deserializeModule();
                sendCanAck();
                sendCanModuleStatus();
            }
            break;
        case CanCommands::Command::RequestStatus:
            if (canCommand->nodeGroupIndex == CanCommands::NoNodeGroup) {
                sendCanAck();
                sendCanModuleStatus();
            } else {
                for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
                    if (s_data.nodeGroups[i].index == canCommand->nodeGroupIndex) {
                        sendCanAck();
                        s_data.nodeGroups[i].usedOnBus = true;
                        return;
                    }
                }
                // If there is not a node group found
                sendCanNak();
            }
            break;
        case CanCommands::Command::RequestDataSize: {
            BinaryArrayDeserializer des(canCommand->data, 8);
            bool fullConfig = false;
            des.readRaw<bool>(&fullConfig);

            sendDataSize(fullConfig);
            break;
        }
        case CanCommands::Command::RequestData: {
            BinaryArrayDeserializer des(canCommand->data, 8);
            uint32_t offset = 0;
            des.readRaw<uint32_t>(&offset);
            bool fullConfig = false;
            des.readRaw<bool>(&fullConfig);

            sendData(offset, fullConfig);
            break;
        }
        case CanCommands::Command::ReserveConfigSize:
            BrytecBoard::ReserveConfigSize(canCommand->nodeGroupIndex);
            sendCanAck();
            break;
        case CanCommands::Command::WriteConfigData:
            if (s_data.mode == EBrytecApp::Mode::Programming) {
                uint16_t offset = canCommand->nodeGroupIndex;
                BrytecBoard::updateConfig(canCommand->data, 8, offset);
                sendCanAck();
            } else
                sendCanNak();
            break;
        default:
            sendCanNak();
            break;
        }
    }
}

void EBrytecApp::setMode(Mode mode)
{
    switch (mode) {
    case Mode::Normal:
        setupModule();
        setupPins();
        // Normal mode only if deserialize is ok
        if(s_data.deserializeOk)
            s_data.mode = mode;
        break;
    case Mode::Stopped:
        BrytecBoard::shutdownAllPins();
        s_data.mode = mode;
        break;
    case Mode::Programming:
        s_data.deserializeOk = false;
        s_data.mode = mode;
        break;
    }
}

void EBrytecApp::sendCanNak()
{
    CanCommands command;
    command.command = CanCommands::Command::Nak;
    command.moduleAddress = s_data.moduleAddress;
    BrytecBoard::sendBrytecCan(command.getFrame());
}

void EBrytecApp::sendCanAck()
{
    CanCommands command;
    command.command = CanCommands::Command::Ack;
    command.moduleAddress = s_data.moduleAddress;
    BrytecBoard::sendBrytecCan(command.getFrame());
}

void EBrytecApp::sendCanModuleStatus()
{
    ModuleStatusBroadcast bc;
    bc.moduleAddress = s_data.moduleAddress;
    bc.mode = (uint8_t)s_data.mode;
    bc.deserializeOk = s_data.deserializeOk;
    bc.nodeArraySize = s_data.nodeVector.getSize();
    BrytecBoard::sendBrytecCan(bc.getFrame());
}

void EBrytecApp::sendDataSize(bool fullConfig)
{
    CanCommands command;
    command.command = CanCommands::Command::RequestDataSize;
    command.moduleAddress = s_data.moduleAddress;
    uint32_t size = 0;
    if (fullConfig)
        size = BrytecBoard::getConfigSize();
    else
        size = BrytecBoard::getTemplateSize();

    BinaryBufferSerializer ser(command.data, 8);
    ser.writeRaw<uint32_t>(size);
    ser.writeRaw<bool>(fullConfig);

    BrytecBoard::sendBrytecCan(command.getFrame());
}

void EBrytecApp::sendData(uint32_t offset, bool fullConfig)
{
    CanCommands command;
    command.command = CanCommands::Command::RequestData;
    command.moduleAddress = s_data.moduleAddress;
    if (fullConfig)
        BrytecBoard::getConfigData(command.data, offset, 8);
    else
        BrytecBoard::getTemplateData(command.data, offset, 8);

    BrytecBoard::sendBrytecCan(command.getFrame());
}
}