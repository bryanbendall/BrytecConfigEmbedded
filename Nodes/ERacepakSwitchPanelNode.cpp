#include "ERacepakSwitchPanelNode.h"

namespace Brytec {

template <>
ENode* CreateNodeInPlace<NodeTypes::Racepak_Switch_Panel>(const ENodeSpec& spec, uint8_t* destination)
{
    if (spec.type != NodeTypes::Racepak_Switch_Panel || spec.numInputs != 8 || spec.numValues != 0)
        return nullptr;

    // Not using ENODE_FULL_TEMPLATE beacuse it will be to big with so many inputs
    return new (destination) ERacepakSwitchPanelNode();
}

}