#pragma once

#include "Boards/BrytecBoard.h"
#include "Can/EBrytecCan.h"
#include "Can/ECanHolleyBroadcastQueue.h"
#include "Deserializer/BinaryDeserializer.h"
#include "Usb/UsbDefs.h"
#include "Utils/ENodeGroup.h"
#include "Utils/ENodesVector.h"
#include <stdint.h>

namespace Brytec {

class ENodeGroupNode;
class NodeGroup;

class EBrytecApp {

public:
    enum Mode : uint8_t {
        Stopped,
        Normal
    };

public:
    static void initalize();
    static bool isDeserializeOk();
    static void update(uint32_t timestepMs);
    static void canReceived(uint8_t canIndex, const CanFrame& frame);
    static void brytecUsbReceived(const Brytec::UsbPacket& packet);
    static void processCanCommands();
    static CanFrame getCustomCanFrame(uint8_t canIndex, uint32_t index);
    static const ECanHolleyBroadcastQueue& getHolleyQueue();
    static uint8_t findCanIndex(CanTypes::Types type);

private:
    static void setMode(Mode mode);
    static void deserializeModule();
    static void setupModule();
    static void setupPins();
    static void setupHolleyBroadcastQueue();
    static void setupCustomCanInputQueue();
    static bool checkAndAssignNodeGroup(ENodeGroupNode* nodeGroupNode);
    static ENode* getNode(int index);
    static ENode* getInitialValueNode(int startIndex, int nodeCount);
    static ENode* getPreviousValueNode(int startIndex, int nodeCount);
    static ENode* getFinalValueNode(int startIndex, int nodeCount);
    static ENode* getPinCurrentNode(int startIndex, int nodeCount);
    static void sendBrytecCan(const CanFrame& frame, bool toUsb = true);
    static void sendBrytecCanBroadcasts();
    static void sendBrytecCanPinStatus(ENodeGroup& nodeGroup);
    static void sendBrytecAddressRequests();
    static void sendRacepakCan(uint32_t timestepMs);
    static void clearNodeGroupNodeAddresses();
    static void queueBrytecCanMessage(const CanFrame& frame);
    static void updateNodeGroupNodes(uint32_t timestepMs);
    static void updatePreviousValueNodes(uint32_t timestepMs);
    static void updateHolleyBroadcastNodes(uint32_t timestepMs);
    static void updateCustomCanInputNodes();
    static void updateCurrents(uint32_t timestepMs);
    static void evaulateJustNodes(uint32_t timestepMs);
    static void sendCanNak();
    static void sendCanAck();
    static void sendCanModuleStatus();
    static void sendDataSize(bool fullConfig);
    static void sendData(uint32_t offset, bool fullConfig);

    friend class NodeGroup;
    friend class ENodeGroup;
};

}