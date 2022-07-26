#pragma once

#include "Boards/BrytecBoard.h"
#include "BrytecConfigEmbedded/Utils/NodesVector.h"
#include "Utils/BinaryDeserializer.h"
#include <stdint.h>

class EBrytecApp {

public:
    static void deserializeModule(BinaryDeserializer& des);
    static void setupPins();
    static void update(float timestep);
    static ENode* getNode(int index);
    static ENode* getInitialValueNode(int startIndex, int nodeCount);
    static ENode* getFinalValueNode(int startIndex, int nodeCount);

private:
    static void evaulateJustNodes(float timestep);

    friend class NodeGroup;
};

// void testMain()
// {

//     // start up stuff

//     BinaryDeserializer ser(nullptr);
//     EBrytecApp::deserializeModule(ser);
//     EBrytecApp::setupPins();

//     while (1) {

//         // board stuff

//         EBrytecApp::update(2.0f);
//     }
// }