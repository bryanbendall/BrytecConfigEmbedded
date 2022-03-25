#include "ENode.h"
#include "Nodes/EAndNode.h"
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
    sizeof(int), // Final_Value = 0,
    sizeof(int), // Initial_Value,
    sizeof(int), // Node_Group, // TODO
    sizeof(EAndNode),
    sizeof(int), // Or,
    sizeof(int), // Two_Stage,
    sizeof(int), // Curve,
    sizeof(int), // Compare,
    sizeof(int), // On_Off,
    sizeof(int), // Invert,
    sizeof(int), // Toggle,
    sizeof(int), // Delay,
    sizeof(int), // Push_Button,
    sizeof(int), // Map_Value,
    sizeof(int), // Math,
    sizeof(int), // Value,
    sizeof(int), // Switch,
    sizeof(int), // CanBus,
};