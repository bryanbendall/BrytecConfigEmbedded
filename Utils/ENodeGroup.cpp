#include "ENodeGroup.h"

#include "Boards/BrytecBoard.h"
#include "EBrytecApp.h"
#include "ENode.h"

void ENodeGroup::setupPin()
{
    if (!enabled)
        return;

    BrytecBoard::setupPin(boardPinIndex, type);
}

void ENodeGroup::updateInitialValue()
{
    if (!enabled)
        return;

    ENode* node = EBrytecApp::getInitialValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    node->SetValue(0, BrytecBoard::getPinValue(boardPinIndex));
}

void ENodeGroup::updateFinalValue()
{
    if (!enabled)
        return;

    ENode* node = EBrytecApp::getFinalValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    float* output = node->GetOutput();
    if (!output)
        return;

    BrytecBoard::setPinValue(boardPinIndex, type, *output);
}

float ENodeGroup::getFinalValue()
{
    if (!enabled)
        return 0.0f;

    ENode* node = EBrytecApp::getFinalValueNode(startNodeIndex, nodeCount);
    if (!node)
        return 0.0f;

    float* output = node->GetOutput();
    if (!output)
        return 0.0f;

    return *output;
}