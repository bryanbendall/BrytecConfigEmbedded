#include "EDeltaTimeNode.h"

namespace Brytec {

ENode* EDeltaTimeNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::DeltaTime || spec.numInputs != 0 || spec.numValues != 0)
        return nullptr;

    return new (destination) EDeltaTimeNodeInternal();

    return nullptr;
}

}