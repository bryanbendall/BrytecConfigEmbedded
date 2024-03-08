#include "EOrNode.h"

namespace Brytec {

ENode* EOrNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Or || spec.numInputs != 5 || spec.numValues != 0)
        return nullptr;

    return new (destination) EOrNodeInternal();
}

}