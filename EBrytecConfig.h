#pragma once

namespace Brytec {

// #define BRYTEC_EMBEDDED

// Nodes will use more ram but the program size will be a lot less
#define ENODE_FULL_TEMPLATE

#define NODES_VECTOR_SIZE_BYTES 1000

#define MAX_NODE_SIZE 64

#define CAN_BUS_BROADCAST_BUFFER 2

#define CAN_UPDATE_FREQUENCY 0.1f

}