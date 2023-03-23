#pragma once

namespace Brytec {

enum class EBrytecErrors {
    ModuleHeader,
    NodeGroupHeader,
    ModuleNotEnabled,
    AddNodeFailed,
    NodeVectorOutOfSpace,
    FailedToCreateNode,
    NodeIndexOutOfBounds,
    BadAlloc,
    CanBufferFull
};

}