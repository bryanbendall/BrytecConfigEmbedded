#pragma once

#include "ENode.h"
#include "Utils/BinaryDeserializer.h"

class ENodeDeserializer {
public:
    static ENodeSpec deserializeNodeSpec(BinaryDeserializer& des);
    static ENodeConnection deserializeNodeConnection(BinaryDeserializer& des);
};