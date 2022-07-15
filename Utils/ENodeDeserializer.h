#pragma once

#include "BrytecConfigEmbedded/ENode.h"
#include "BrytecConfigEmbedded/Utils/BinaryDeserializer.h"

class ENodeDeserializer {
public:
    static ENodeSpec deserializeNodeSpec(BinaryDeserializer& des);
    static ENodeConnection deserializeNodeConnection(BinaryDeserializer& des);
};