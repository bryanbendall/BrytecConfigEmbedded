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
    case NodeTypes::Final_Value:
        return new (ptr) EFinalValueNode();
    case NodeTypes::Initial_Value:
        return new (ptr) EInitialValueNode();
    case NodeTypes::Node_Group:
        return new (ptr) ENodeGroupNode();
    case NodeTypes::And:
        return new (ptr) EAndNode();
    case NodeTypes::Or:
        return new (ptr) EOrNode();
    case NodeTypes::Two_Stage:
        return new (ptr) ETwoStageNode();
    case NodeTypes::Curve:
        return new (ptr) ECurveNode();
    case NodeTypes::Compare:
        return new (ptr) ECompareNode();
    case NodeTypes::On_Off:
        return new (ptr) EOnOffNode();
    case NodeTypes::Invert:
        return new (ptr) EInvertNode();
    case NodeTypes::Toggle:
        return new (ptr) EToggleNode();
    case NodeTypes::Delay:
        return new (ptr) EDelayNode();
    case NodeTypes::Push_Button:
        return new (ptr) EPushButtonNode();
    case NodeTypes::Map_Value:
        return new (ptr) EMapValueNode();
    case NodeTypes::Math:
        return new (ptr) EMathNode();
    case NodeTypes::Value:
        return new (ptr) EValueNode();
    case NodeTypes::Switch:
        return new (ptr) ESwitchNode();
    case NodeTypes::CanBus:
        return new (ptr) ECanBusNode();

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