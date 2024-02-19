#pragma once

#if __has_include("EBrytecConfigCustom.h")
#include "EBrytecConfigCustom.h"
#endif

namespace Brytec {

// Nodes will use more ram but the program size will be a lot less
#ifndef ENODE_FULL_TEMPLATE
#define ENODE_FULL_TEMPLATE 1
#endif

#ifndef NODES_VECTOR_SIZE_BYTES
#define NODES_VECTOR_SIZE_BYTES 5000
#endif

#ifndef CAN_BUS_BROADCAST_BUFFER
#define CAN_BUS_BROADCAST_BUFFER 100
#endif

#ifndef CAN_UPDATE_FREQUENCY
#define CAN_UPDATE_FREQUENCY 0.1f
#endif

#ifndef USB_BUFFER_SIZE
#define USB_BUFFER_SIZE 100
#endif

#ifndef MAX_CAN_BUSES
#define MAX_CAN_BUSES 1
#endif

#define MAX_NODE_SIZE 64

#define DEFAULT_BRYTEC_CAN_SPEED CanSpeed::Types::Speed_500kBps
}