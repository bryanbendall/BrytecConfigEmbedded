#include "EHolleyBroadcastNode.h"

namespace Brytec {

ENode* EHolleyBroadcastNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{

    if (spec.type != NodeTypes::Holley_Broadcast || spec.numInputs != 0 || spec.numValues != 1)
        return nullptr;

    return new (destination) EHolleyBroadcastNodeInternal();
}

}