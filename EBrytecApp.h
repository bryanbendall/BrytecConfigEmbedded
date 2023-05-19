#pragma once

#include "Boards/BrytecBoard.h"
#include "Can/EBrytecCan.h"
#include "Deserializer/BinaryDeserializer.h"
#include "Utils/ENodeGroup.h"
#include "Utils/ENodesVector.h"
#include <stdint.h>

namespace Brytec{

class ENodeGroupNode;
class NodeGroup;

class EBrytecApp {

public:
    enum Mode : uint8_t {
        Stopped,
        Normal,
        Programming
    };

public:
    static void deserializeModule();
    static bool isDeserializeOk();
    static void setMode(Mode mode);
    static void update(float timestep);
    static void brytecCanReceived(const CanExtFrame& frame);

private:
    static void setupModule();
    static void setupPins();
    static ENode* getNode(int index);
    static ENode* getInitialValueNode(int startIndex, int nodeCount);
    static ENode* getFinalValueNode(int startIndex, int nodeCount);
    static ENode* getPinCurrentNode(int startIndex, int nodeCount);
    static void sendBrytecCanBroadcasts();
    static void sendBrytecCanPinStatus(ENodeGroup& nodeGroup);
    static ENodeGroupNode* findNodeGroupNode(uint8_t moduleAddress, uint16_t nodeGroupIndex);
    static void updateNodeGroupNodes();
    static void updateCurrents(float timestep);
    static void evaulateJustNodes(float timestep);
    static void calculateMaskAndFilter(uint32_t* mask, uint32_t* filter);

    static void processCanCommands();
    static void sendCanNak();
    static void sendCanAck();
    static void sendCanModuleStatus();

    friend class NodeGroup;
    friend class ENodeGroup;
};

}