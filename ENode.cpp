#include "ENode.h"
#include "Nodes/EAndNode.h"
#include "Nodes/ECanBusNode.h"
#include "Nodes/ECompareNode.h"
#include "Nodes/ECurveNode.h"
#include "Nodes/EDelayNode.h"
#include "Nodes/EFinalValueNode.h"
#include "Nodes/EInitialValueNode.h"
#include "Nodes/EInvertNode.h"
#include "Nodes/EMapValueNode.h"
#include "Nodes/EMathNode.h"
#include "Nodes/ENodeGroupNode.h"
#include "Nodes/EOnOffNode.h"
#include "Nodes/EOrNode.h"
#include "Nodes/EPushButtonNode.h"
#include "Nodes/ESwitchNode.h"
#include "Nodes/EToggleNode.h"
#include "Nodes/ETwoStageNode.h"
#include "Nodes/EValueNode.h"
#include <new>

bool ToBool(float& f)
{
    if (f >= 0.0001f)
        return true;
    else
        return false;
}

ENode* ENode::create(NodeTypes type, void* ptr)
{
    switch (type) {
    case NodeTypes::And:
        return new (ptr) EAndNode();

    default:
        return nullptr;
    }
}

const uint32_t ENode::m_sizes[] = {
    sizeof(EFinalValueNode),
    sizeof(EInitialValueNode),
    sizeof(ENodeGroupNode),
    sizeof(EAndNode),
    sizeof(EOrNode),
    sizeof(ETwoStageNode),
    sizeof(ECurveNode),
    sizeof(ECompareNode),
    sizeof(EOnOffNode),
    sizeof(EInvertNode),
    sizeof(EToggleNode),
    sizeof(EDelayNode),
    sizeof(EPushButtonNode),
    sizeof(EMapValueNode),
    sizeof(EMathNode),
    sizeof(EValueNode),
    sizeof(ESwitchNode),
    sizeof(ECanBusNode),
};