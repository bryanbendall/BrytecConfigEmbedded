#pragma once

#include "Boards/BrytecBoard.h"
#include "Can/EBrytecCan.h"
#include "Deserializer/BinaryDeserializer.h"
#include "Utils/ENodesVector.h"
#include <stdint.h>

class ENodeGroupNodeInternal;

class EBrytecApp {

public:
    static void deserializeModule(BinaryDeserializer& des);
    static bool isDeserializeOk();
    static void setupModule();
    static void setupPins();
    static void update(float timestep);
    static ENode* getNode(int index);
    static ENode* getInitialValueNode(int startIndex, int nodeCount);
    static ENode* getFinalValueNode(int startIndex, int nodeCount);
    static ENode* getPinCurrentNode(int startIndex, int nodeCount);
    static void brytecCanReceived(const EBrytecCan::CanExtFrame& frame);
    static void sendBrytecCanBroadcasts();

private:
    static ENodeGroupNodeInternal* findNodeGroupNode(uint8_t moduleAddress, uint16_t nodeGroupIndex);
    static void updateNodeGroupNodes();
    static void updateCurrents(float timestep);
    static void evaulateJustNodes(float timestep);

    friend class NodeGroup;
};
