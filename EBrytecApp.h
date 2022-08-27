#pragma once

#include "Boards/BrytecBoard.h"
#include "Utils/BinaryDeserializer.h"
#include "Utils/ENodesVector.h"
#include <stdint.h>

class EBrytecApp {

public:
    static void deserializeModule(BinaryDeserializer& des);
    static void setupModule();
    static void setupPins();
    static void update(float timestep);
    static ENode* getNode(int index);
    static ENode* getInitialValueNode(int startIndex, int nodeCount);
    static ENode* getFinalValueNode(int startIndex, int nodeCount);
    static float getBrytecNetworkValue(uint8_t moduleAddress, uint8_t pinIndex);

private:
    static void updateNodeGroupNodes();
    static void evaulateJustNodes(float timestep);

    friend class NodeGroup;
};

// void testMain()
// {

//     // start up stuff

//     BinaryDeserializer ser(nullptr);
//     EBrytecApp::deserializeModule(ser);
//     EBrytecApp::setupModule();
//     EBrytecApp::setupPins();

//     while (1) {

//         // board stuff

//         EBrytecApp::update(2.0f);
//     }
// }