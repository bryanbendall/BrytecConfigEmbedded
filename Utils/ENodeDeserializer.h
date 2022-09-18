#pragma once

#include "Deserializer/BinaryDeserializer.h"
#include "ENode.h"

class ENodeDeserializer {
public:
    static ENodeSpec deserializeNodeSpec(BinaryDeserializer& des);
    static ENodeConnection deserializeNodeConnection(BinaryDeserializer& des);
};