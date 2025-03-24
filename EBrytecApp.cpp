#include "EBrytecApp.h"

#include "Can/ECanBus.h"
#include "Can/ECanCommandQueue.h"
#include "Can/ECustomCanQueue.h"
#include "Can/EPinStatusQueue.h"
#include "Deserializer/BinaryArrayDeserializer.h"
#include "Deserializer/BinaryBufferSerializer.h"
#include "Nodes/ECanBusInputNode.h"
#include "Nodes/EFinalValueNode.h"
#include "Nodes/EHolleyBroadcastNode.h"
#include "Nodes/EInitialValueNode.h"
#include "Nodes/ENodeGroupNode.h"
#include "Nodes/ERacepakSwitchPanelNode.h"
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
    ECanBus canBuses[MAX_CAN_BUSES] = {};
    ECanHolleyBroadcastQueue holleyBcQueue;
    ECustomCanQueue customCanInputQueue[MAX_CAN_BUSES];
};
static EBrytecAppData s_data;

void EBrytecApp::initalize()
{
    setMode(Mode::Normal);

    if (!s_data.deserializeOk)
        BrytecBoard::setupCan(0, DEFAULT_BRYTEC_CAN_SPEED);
    // Setup first can with default address so it can be programmed
}

bool EBrytecApp::isDeserializeOk()
{
    return s_data.deserializeOk;
}

void EBrytecApp::update(uint32_t timestepMs)
{
    processCanCommands();

    // Only update in normal mode
    if (s_data.mode != EBrytecApp::Mode::Normal)
        return;

    if (!s_data.deserializeOk)
        return;

    // Update current and over current
    updateCurrents(timestepMs);

    updateNodeGroupNodes(timestepMs);

    updateHolleyBroadcastNodes(timestepMs);

    updateCustomCanInputNodes();

    // Get inputs
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateInitialValue();
    }

    // Calculate all nodes
    s_data.nodeVector.evaluateAll(timestepMs);

    // Set outputs
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
        s_data.nodeGroups[i].updateFinalValue();
    }

    // Send can data
    static uint32_t canDataTimer = 0;
    canDataTimer += timestepMs;
    if (canDataTimer >= CAN_UPDATE_FREQUENCY) {
        sendBrytecCanBroadcasts();
        canDataTimer = 0;
    }

    sendRacepakCan(timestepMs);
}

void EBrytecApp::canReceived(uint8_t canIndex, const CanFrame& frame)
{
    if (canIndex >= MAX_CAN_BUSES)
        return;

    switch (s_data.canBuses[canIndex].type) {
    case CanTypes::Types::Brytec:
        if (frame.type == CanFrameType::Ext) {
            queueBrytecCanMessage(frame);
            BrytecBoard::sendBrytecCanUsb(frame);
        }
        break;

    case CanTypes::Types::Holley: {
        s_data.holleyBcQueue.update(frame);
        break;
    }

    default:
        break;
    }

    // Handle custom can
    if (s_data.customCanInputQueue[canIndex].getSize() > 0)
        s_data.customCanInputQueue[canIndex].update(frame);
}

void EBrytecApp::brytecUsbReceived(const Brytec::UsbPacket& packet)
{
    Brytec::CanFrame frame = packet.as<Brytec::CanFrame>();
    if (frame) {
        queueBrytecCanMessage(frame);
        sendBrytecCan(frame);
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
        if (canCommand->moduleAddress != s_data.moduleAddress && canCommand->moduleAddress != CanCommands::AllModules
            && canCommand->command != CanCommands::RequestAddress)
            continue;

        switch (canCommand->command) {
        case CanCommands::Command::ChangeMode:
            setMode((EBrytecApp::Mode)canCommand->data[0]);
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
            if (s_data.mode == EBrytecApp::Mode::Stopped) {
                s_data.deserializeOk = false;
                setMode(Mode::Normal);
                sendCanModuleStatus();
            }
            break;

        case CanCommands::Command::RequestStatus:
            if (canCommand->nodeGroupIndex == CanCommands::NoNodeGroup) {
                sendCanModuleStatus();
            } else {
                for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
                    if (s_data.nodeGroups[i].index == canCommand->nodeGroupIndex) {
                        printf("request status for - %u", canCommand->nodeGroupIndex);
                        s_data.nodeGroups[i].usedOnBus = canCommand->data[0];
                        return;
                    }
                }
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
            if (s_data.mode == EBrytecApp::Mode::Stopped) {
                s_data.deserializeOk = false;
                uint16_t offset = canCommand->nodeGroupIndex;
                BrytecBoard::updateConfig(canCommand->data, 8, offset);
                sendCanAck();
            } else
                sendCanNak();
            break;

        case CanCommands::Command::RequestAddress: {
            BinaryArrayDeserializer des(canCommand->data, 8);
            uint64_t uuid;
            des.readRaw<uint64_t>(&uuid);

            if (canCommand->nodeGroupIndex == CanCommands::NoNodeGroup) {
                // If we have the node group with this uuid send address
                for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
                    ENodeGroup& ng = s_data.nodeGroups[i];
                    if (ng.uuid == uuid) {
                        // Send address
                        CanCommands returnCommand;
                        returnCommand.command = CanCommands::Command::RequestAddress;
                        returnCommand.moduleAddress = s_data.moduleAddress;
                        returnCommand.nodeGroupIndex = ng.index;
                        BinaryBufferSerializer ser(returnCommand.data, 8);
                        ser.writeRaw<uint64_t>(uuid);
                        sendBrytecCan(returnCommand.getFrame());

                        // If we request the address it must be used on the bus
                        ng.usedOnBus = true;
                    }
                }

            } else {
                // Set all node group nodes with addresses
                for (ENode& node : s_data.nodeVector) {
                    if (node.NodeType() == NodeTypes::Node_Group) {
                        ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;
                        if (nodeGroupNode->getUuid() == uuid) {
                            nodeGroupNode->setModuleAddress(canCommand->moduleAddress);
                            nodeGroupNode->setNodeGroupIndex(canCommand->nodeGroupIndex);
                        }
                    }
                }
            }
        } break;

        default:
            sendCanNak();
            break;
        }
    }
}

CanFrame EBrytecApp::getCustomCanFrame(uint8_t canIndex, uint32_t index)
{
    if (canIndex >= MAX_CAN_BUSES)
        return CanFrame();

    return s_data.customCanInputQueue[canIndex].getFrame(index);
}

const ECanHolleyBroadcastQueue& EBrytecApp::getHolleyQueue()
{
    return s_data.holleyBcQueue;
}

void EBrytecApp::setMode(Mode mode)
{
    switch (mode) {
    case Mode::Normal:
        if (!s_data.deserializeOk) {
            deserializeModule();
            setupModule();
            setupPins();
        }

        if (s_data.deserializeOk)
            s_data.mode = mode;

        break;
    case Mode::Stopped:
        BrytecBoard::shutdownAllPins();
        clearNodeGroupNodeAddresses();
        s_data.mode = mode;
        break;
    }
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

    // Can Bus
    uint8_t canCount;
    des->readRaw<uint8_t>(&canCount);
    for (int c = 0; c < canCount; c++) {
        uint8_t type;
        des->readRaw<uint8_t>(&type);

        uint8_t speed;
        des->readRaw<uint8_t>(&speed);

        if (c < MAX_CAN_BUSES) {
            s_data.canBuses[c].type = (CanTypes::Types)type;
            s_data.canBuses[c].speed = (CanSpeed::Types)speed;
        }
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
#if false //__has_include(<string>)
        std::string ngName;
        des->readRaw<std::string>(&ngName);
        BrytecBoard::AddedNamesNodeGroup(nodeGroupIndex, ngName);
#else
        EmptyString empty;
        des->readRaw<EmptyString>(&empty);
#endif

        // Node Group uuid
        des->readRaw<uint64_t>(&currentNodeGroup->uuid);

        // Node Group type
        uint8_t type;
        des->readRaw<uint8_t>(&type);
        currentNodeGroup->type = (IOTypes::Types)type;

        // Node Group enabled
        uint8_t enabled;
        des->readRaw<uint8_t>(&enabled);
        currentNodeGroup->enabled = enabled;

        // Node Group used on bus (not used because it is requested, TODO: Remove from config)
        uint8_t usedOnBus;
        des->readRaw<uint8_t>(&usedOnBus);

        // Current limit
        des->readRaw<uint8_t>(&currentNodeGroup->currentLimit);

        uint8_t alwaysRetry;
        des->readRaw<uint8_t>(&alwaysRetry);
        currentNodeGroup->alwaysRetry = alwaysRetry;

        des->readRaw<uint8_t>(&currentNodeGroup->maxRetries);

        float retryDelay;
        des->readRaw<float>(&retryDelay);
        currentNodeGroup->retryDelay = FloatTimeToMs(retryDelay);

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

    // Assign node group if it is in this module
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;
            checkAndAssignNodeGroup(nodeGroupNode);
        }
    }

    setupHolleyBroadcastQueue();

    setupCustomCanInputQueue();

    s_data.deserializeOk = true;
}

void EBrytecApp::setupModule()
{
    if (!s_data.deserializeOk)
        return;

    for (int i = 0; i < MAX_CAN_BUSES; i++) {
        if (s_data.canBuses[i].type != CanTypes::Types::Disabled)
            BrytecBoard::setupCan(i, s_data.canBuses[i].speed);
    }
}

void EBrytecApp::setupPins()
{
    if (!s_data.deserializeOk)
        return;

    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].setupPin();
}

void EBrytecApp::setupHolleyBroadcastQueue()
{
    // If we don't have a Holley can setup don't do anything
    bool hasHolleyCan = false;
    for (int i = 0; i < MAX_CAN_BUSES; i++) {
        if (s_data.canBuses[i].type == CanTypes::Types::Holley)
            hasHolleyCan = true;
    }
    if (!hasHolleyCan)
        return;

    // Find total holley node count
    uint32_t holleyNodeCount = 0;
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Holley_Broadcast)
            holleyNodeCount++;
    }

    // Early out if we dont have any nodes
    if (holleyNodeCount == 0)
        return;

    // Buffer to hold channel numbers
    uint32_t* channelBuffer = (uint32_t*)malloc(sizeof(uint32_t) * holleyNodeCount);
    for (uint32_t i = 0; i < holleyNodeCount; i++)
        channelBuffer[i] = UINT32_MAX;

    // Find empty channel or the same channel to eliminate doubles
    uint32_t trimmedHolleyNodeCount = 0;
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Holley_Broadcast) {
            for (uint32_t j = 0; j < holleyNodeCount; j++) {
                uint32_t channel = FloatToInt(node.GetValue(0));
                if ((channelBuffer[j] == UINT32_MAX) || (channelBuffer[j] == channel)) {
                    EHolleyBroadcastNodeInternal* holleyNode = (EHolleyBroadcastNodeInternal*)&node;
                    holleyNode->setCanFrameIndex(j);
                    if (channelBuffer[j] == UINT32_MAX) {
                        channelBuffer[j] = channel;
                        trimmedHolleyNodeCount++;
                    }
                    j = holleyNodeCount; // Break from loop
                }
            }
        }
    }

    // Add channel to queue
    s_data.holleyBcQueue.init(trimmedHolleyNodeCount);
    for (uint32_t i = 0; i < trimmedHolleyNodeCount; i++)
        s_data.holleyBcQueue.insert(i, channelBuffer[i]);

    free(channelBuffer);
}

void EBrytecApp::setupCustomCanInputQueue()
{
    // Count ECanBusNodes per can channel
    uint32_t nodeCounts[MAX_CAN_BUSES] = { 0 };
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::CanBusInput) {
            // ECanBusNode* canBusNode = (ECanBusNode*)&node;
            uint8_t canIndex = node.GetValue(1);
            if (canIndex > MAX_CAN_BUSES)
                continue;
            nodeCounts[canIndex]++;
        }
    }

    for (uint8_t canBusIndex = 0; canBusIndex < MAX_CAN_BUSES; canBusIndex++) {

        // We dont have any nodes for this can bus
        if (nodeCounts[canBusIndex] == 0)
            continue;

        // Buffer to count id and skip doubles
        uint32_t* tempBuffer = (uint32_t*)malloc(sizeof(uint32_t) * nodeCounts[canBusIndex]);
        for (uint32_t i = 0; i < nodeCounts[canBusIndex]; i++)
            tempBuffer[i] = UINT32_MAX;

        // Find empty id or the same id to eliminate doubles
        uint32_t trimmedCount = 0;
        for (ENode& node : s_data.nodeVector) {
            if (node.NodeType() == NodeTypes::CanBusInput) {
                for (uint32_t j = 0; j < nodeCounts[canBusIndex]; j++) {
                    // Check against can bus id
                    uint32_t id = FloatToInt(node.GetValue(0));
                    if ((tempBuffer[j] == UINT32_MAX) | (tempBuffer[j] == id)) {
                        tempBuffer[j] = id;
                        trimmedCount++;
                        ECanBusInputNodeInternal* canNode = (ECanBusInputNodeInternal*)&node;
                        canNode->setCanFrameIndex(j);
                        j = nodeCounts[canBusIndex]; // Break from loop
                    }
                }
            }
        }

        // Add id to queue
        s_data.customCanInputQueue[canBusIndex].init(trimmedCount);
        for (uint32_t i = 0; i < trimmedCount; i++)
            s_data.customCanInputQueue[canBusIndex].insert(i, tempBuffer[i]);

        free(tempBuffer);
    }
}

bool EBrytecApp::checkAndAssignNodeGroup(ENodeGroupNode* nodeGroupNode)
{
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++) {
        if (nodeGroupNode->getUuid() == s_data.nodeGroups[i].uuid) {
            nodeGroupNode->setModuleAddress(s_data.moduleAddress);
            nodeGroupNode->setNodeGroupIndex(i);
            return true;
        }
    }

    return false;
}

ENode* EBrytecApp::getNode(int index)
{
    return s_data.nodeVector.at(index);
}

ENode* EBrytecApp::getInitialValueNode(int startIndex, int nodeCount)
{
    ENodesVector::Iterator node(s_data.nodeVector.at(startIndex));
    for (int i = 0; i < nodeCount; i++, node++) {
        if (node->NodeType() == NodeTypes::Initial_Value)
            return &*node;
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
    ENodesVector::Iterator node(s_data.nodeVector.at(startIndex));
    for (int i = 0; i < nodeCount; i++, node++) {
        if (node->NodeType() == NodeTypes::PinCurrent)
            return &*node;
    }

    return nullptr;
}

void EBrytecApp::sendBrytecCan(const CanFrame& frame)
{
    for (int i = 0; i < MAX_CAN_BUSES; i++) {
        if (s_data.canBuses[i].type == CanTypes::Types::Brytec)
            BrytecBoard::sendCan(i, frame);
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

    CanFrame frame = pinStatus.getFrame();
    sendBrytecCan(frame);
    BrytecBoard::sendBrytecCanUsb(frame);
}

void EBrytecApp::sendRacepakCan(uint32_t timestepMs)
{
    static uint32_t racepakTimer = 0;
    racepakTimer += timestepMs;

    // Early out if not time to send
    if (racepakTimer < 100)
        return;

    racepakTimer = 0;

    for (uint8_t canIndex = 0; canIndex < MAX_CAN_BUSES; canIndex++) {
        if (s_data.canBuses[canIndex].type == CanTypes::Types::Racepak) {

            for (ENode& node : s_data.nodeVector) {
                if (node.NodeType() == NodeTypes::Racepak_Switch_Panel) {
                    ERacepakSwitchPanelNode* racepakNode = (ERacepakSwitchPanelNode*)&node;

                    // Read all switch values
                    uint8_t switches = 0;
                    for (uint8_t j = 0; j < 8; j++) {
                        if (racepakNode->GetValue(j) > 0.001f)
                            switches |= (1 << j);
                    }

                    // Send can message
                    CanFrame frame;
                    frame.id = 0x194;
                    frame.type = CanFrameType::Std;
                    frame.data[3] = switches;
                    // TODO needs testing!!!!
                    BrytecBoard::sendCan(canIndex, frame);
                }
            }
        }
    }
}

void EBrytecApp::sendBrytecAddressRequests()
{
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;
            // Only request the address of ones we haven't found yet and has a uuid
            if (nodeGroupNode->getModuleAddress() == CanCommands::AllModules
                && nodeGroupNode->getNodeGroupIndex() == CanCommands::NoNodeGroup
                && nodeGroupNode->getUuid() != 0) {
                CanCommands cmd;
                cmd.command = CanCommands::Command::RequestAddress;
                cmd.moduleAddress = s_data.moduleAddress;
                BinaryBufferSerializer ser(cmd.data, 8);
                ser.writeRaw<uint64_t>(nodeGroupNode->getUuid());
                sendBrytecCan(cmd.getFrame());
            }
        }
    }
}

void EBrytecApp::clearNodeGroupNodeAddresses()
{
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;

            nodeGroupNode->setModuleAddress(CanCommands::AllModules);
            nodeGroupNode->setNodeGroupIndex(CanCommands::NoNodeGroup);
        }
    }
}

void EBrytecApp::queueBrytecCanMessage(const CanFrame& frame)
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

void EBrytecApp::updateNodeGroupNodes(uint32_t timestepMs)
{
    // Internal nodes to this module
    {
        for (ENode& node : s_data.nodeVector) {
            if (node.NodeType() == NodeTypes::Node_Group) {
                ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;
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
            for (ENode& node : s_data.nodeVector) {
                if (node.NodeType() == NodeTypes::Node_Group) {
                    ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;
                    if (nodeGroupNode->getModuleAddress() == pinStatus->moduleAddress && nodeGroupNode->getNodeGroupIndex() == pinStatus->nodeGroupIndex)
                        nodeGroupNode->SetValue(99, pinStatus->value);
                }
            }
        }
    }

    // Find Node Group Nodes if not found yet
    static uint32_t nodeGroupTimer = 0;
    nodeGroupTimer += timestepMs;

    if (nodeGroupTimer >= 1000) {
        sendBrytecAddressRequests();
        nodeGroupTimer = 0;
    }
}

void EBrytecApp::updateHolleyBroadcastNodes(uint32_t timestepMs)
{
    s_data.holleyBcQueue.updateTimestep(timestepMs);

    // Switch buffers in case we get new messages while we are updating
    s_data.holleyBcQueue.swapBuffers();
}

void EBrytecApp::updateCustomCanInputNodes()
{
    for (uint32_t canBusIndex = 0; canBusIndex < MAX_CAN_BUSES; canBusIndex++) {

        // No nodes for this can index
        if (s_data.customCanInputQueue[canBusIndex].getSize() == 0)
            continue;

        s_data.customCanInputQueue[canBusIndex].swapBuffers();
    }
}

void EBrytecApp::updateCurrents(uint32_t timestepMs)
{
    for (uint16_t i = 0; i < s_data.nodeGroupsCount; i++)
        s_data.nodeGroups[i].updatePinCurrent(timestepMs);
}

void EBrytecApp::evaulateJustNodes(uint32_t timestepMs)
{
    // Calculate all nodes
    s_data.nodeVector.evaluateAll(timestepMs);
}

void EBrytecApp::sendCanNak()
{
    CanCommands command;
    command.command = CanCommands::Command::Nak;
    command.moduleAddress = s_data.moduleAddress;

    CanFrame frame = command.getFrame();
    sendBrytecCan(frame);
    BrytecBoard::sendBrytecCanUsb(frame);
}

void EBrytecApp::sendCanAck()
{
    CanCommands command;
    command.command = CanCommands::Command::Ack;
    command.moduleAddress = s_data.moduleAddress;

    CanFrame frame = command.getFrame();
    sendBrytecCan(frame);
    BrytecBoard::sendBrytecCanUsb(frame);
}

void EBrytecApp::sendCanModuleStatus()
{
    ModuleStatusBroadcast bc;
    bc.moduleAddress = s_data.moduleAddress;
    bc.mode = (uint8_t)s_data.mode;
    bc.deserializeOk = s_data.deserializeOk;
    bc.nodePercent = s_data.nodeVector.getSize() * 100 / NODES_VECTOR_SIZE_BYTES;

    CanFrame frame = bc.getFrame();
    sendBrytecCan(frame);
    BrytecBoard::sendBrytecCanUsb(frame);
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

    CanFrame frame = command.getFrame();
    sendBrytecCan(frame);
    BrytecBoard::sendBrytecCanUsb(frame);
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

    CanFrame frame = command.getFrame();
    sendBrytecCan(frame);
    BrytecBoard::sendBrytecCanUsb(frame);
}
}