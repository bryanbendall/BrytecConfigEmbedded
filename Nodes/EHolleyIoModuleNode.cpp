#include "EHolleyIoModuleNode.h"

namespace Brytec {

ENode* EHolleyIoModuleNode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Holley_Io_Module || spec.numInputs != 1 || spec.numValues != 4)
        return nullptr;

#if ENODE_FULL_TEMPLATE

    auto input0 = spec.connections[0];

    if (input0 == Float)
        return new (destination) EHolleyIoModuleNodeInternal<float>();
    if (input0 == Pointer)
        return new (destination) EHolleyIoModuleNodeInternal<float*>();

#else
    return new (destination) EHolleyIoModuleNodeInternal<float>();
#endif

    return nullptr;
}

}