#pragma once

namespace Brytec {

// Nodes will use more ram but the program size will be a lot less
#define ENODE_FULL_TEMPLATE

#define NODES_VECTOR_SIZE_BYTES 5000

#define MAX_NODE_SIZE 64

#define CAN_BUS_BROADCAST_BUFFER 10

#define CAN_UPDATE_FREQUENCY 0.1f

#define USB_BUFFER_SIZE 100

#define MAX_CAN_BUSES 1

#define DEFAULT_BRYTEC_CAN_SPEED CanSpeed::Types::Speed_500kBps
}