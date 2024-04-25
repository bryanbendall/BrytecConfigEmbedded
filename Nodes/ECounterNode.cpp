#include "ECounterNode.h"

namespace Brytec {

ENode* ECounterNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Counter || spec.numInputs != 5 || spec.numValues != 3)
        return nullptr;

    return new (destination) ECounterNodeInternal();

    return nullptr;
}

}