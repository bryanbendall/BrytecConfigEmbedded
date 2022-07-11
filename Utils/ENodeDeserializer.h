#pragma once

#include "BrytecConfigEmbedded/ENode.h"
#include "BrytecConfigEmbedded/Utils/BinaryDeserializer.h"

class ENodeDeserializer {
public:
    static ENodeSpec deserializeNode(BinaryDeserializer& des);
};