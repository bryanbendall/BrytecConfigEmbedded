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

ENode* ENode::CreateInPlace(const ENodeSpec& spec, uint8_t* destination)
{
    if (!destination)
        return nullptr;

    switch (spec.type) {
    case NodeTypes::Final_Value:
        return EFinalValueNode::CreateInPlace(spec, destination);
    case NodeTypes::Initial_Value:
        return EInitialValueNode::CreateInPlace(spec, destination);
    case NodeTypes::Node_Group:
        return ENodeGroupNode::CreateInPlace(spec, destination);
    case NodeTypes::And:
        return EAndNode::CreateInPlace(spec, destination);
    case NodeTypes::Or:
        return EOrNode::CreateInPlace(spec, destination);
    case NodeTypes::Two_Stage:
        return ETwoStageNode::CreateInPlace(spec, destination);
    case NodeTypes::Curve:
        return ECurveNode::CreateInPlace(spec, destination);
    case NodeTypes::Compare:
        return ECompareNode::CreateInPlace(spec, destination);
    case NodeTypes::On_Off:
        return EOnOffNode::CreateInPlace(spec, destination);
    case NodeTypes::Invert:
        return EInvertNode::CreateInPlace(spec, destination);
    case NodeTypes::Toggle:
        return EToggleNode::CreateInPlace(spec, destination);
    case NodeTypes::Delay:
        return EDelayNode::CreateInPlace(spec, destination);
    case NodeTypes::Push_Button:
        return EPushButtonNode::CreateInPlace(spec, destination);
    case NodeTypes::Map_Value:
        return EMapValueNode::CreateInPlace(spec, destination);
    case NodeTypes::Math:
        return EMathNode::CreateInPlace(spec, destination);
    case NodeTypes::Value:
        return EValueNode::CreateInPlace(spec, destination);
    case NodeTypes::Switch:
        return ESwitchNode::CreateInPlace(spec, destination);
    case NodeTypes::CanBus:
        // return new (ptr) ECanBusNode();

    default:
        return nullptr;
    }
}
