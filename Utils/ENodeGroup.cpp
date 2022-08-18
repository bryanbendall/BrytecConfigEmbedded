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

    BrytecBoard::setPinValue(boardPinIndex, *output);
}