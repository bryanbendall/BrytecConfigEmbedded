#include "EBrytecApp.h"

#include "Can/ECanBus.h"
#include "Can/ECanCommandQueue.h"
#include "Can/ECanHolleyBroadcastQueue.h"
#include "Can/EPinStatusQueue.h"
#include "Deserializer/BinaryArrayDeserializer.h"
#include "Deserializer/BinaryBufferSerializer.h"
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
};
static EBrytecAppData s_data;

void EBrytecApp::initalize()
{
    deserializeModule();
    if (s_data.deserializeOk)
        setMode(Mode::Normal);
    else
        BrytecBoard::setupCan(0, DEFAULT_BRYTEC_CAN_SPEED);
    // Setup first can with default address so it can be programmed
}

bool EBrytecApp::isDeserializeOk()
{
    return s_data.deserializeOk;
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

    // Update Holley Broadcasts
    updateHolleyBroadcastNodes();

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

    sendRacepakCan(timestep);
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
        HolleyBroadcast holleyBc(frame);
        if (!holleyBc)
            break;
        s_data.holleyBcQueue.update(holleyBc);
        break;
    }

    default:
        break;
    }
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
                        s_data.nodeGroups[i].usedOnBus = canCommand->data[0];
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
        if (s_data.deserializeOk)
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

    setupHolleyBroadcastQueue();

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
    // If we don't hava a Holley can setup don't do anything
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

    // Buffer to hold channel numbers
    uint32_t* channelBuffer = (uint32_t*)malloc(sizeof(uint32_t) * holleyNodeCount);
    for (int i = 0; i < holleyNodeCount; i++)
        channelBuffer[i] = UINT32_MAX;

    // Find empty channel or the same channel to eliminate doubles
    uint32_t trimmedHolleyNodeCount = 0;
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Holley_Broadcast) {
            for (int j = 0; j < holleyNodeCount; j++) {
                if (channelBuffer[j] == UINT32_MAX | channelBuffer[j] == node.GetValue(0)) {
                    channelBuffer[j] = node.GetValue(0);
                    trimmedHolleyNodeCount++;
                    break;
                }
            }
        }
    }

    // Add channel to queue
    s_data.holleyBcQueue.init(trimmedHolleyNodeCount);
    for (int i = 0; i < trimmedHolleyNodeCount; i++)
        s_data.holleyBcQueue.insert(i, HolleyBroadcast(channelBuffer[i]));

    free(channelBuffer);
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

void EBrytecApp::sendRacepakCan(float timestep)
{
    static float racepakTimer = 0.0f;
    racepakTimer += timestep;

    // Early out if not time to send
    if (racepakTimer < 0.1f)
        return;

    racepakTimer = 0.0f;

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

ENodeGroupNode* EBrytecApp::findNodeGroupNode(uint8_t moduleAddress, uint16_t nodeGroupIndex)
{
    for (ENode& node : s_data.nodeVector) {
        if (node.NodeType() == NodeTypes::Node_Group) {
            ENodeGroupNode* nodeGroupNode = (ENodeGroupNode*)&node;
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
}

void EBrytecApp::updateHolleyBroadcastNodes()
{
    // Copy queue in case we get new messages while we are updating
    ECanHolleyBroadcastQueue queue = s_data.holleyBcQueue;

    for (int queueIndex = 0; queueIndex < queue.getSize(); queueIndex++) {
        // We need to go through all nodes beacuse we might match more then one
        for (ENode& node : s_data.nodeVector) {
            if (node.NodeType() == NodeTypes::Holley_Broadcast) {
                EHolleyBroadcastNode* holleyNode = (EHolleyBroadcastNode*)&node;
                *holleyNode->GetOutput(0) = queue.getValue(queueIndex);
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
    bc.nodeArraySize = s_data.nodeVector.getSize();

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