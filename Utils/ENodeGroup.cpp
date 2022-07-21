#include "ENodeGroup.h"

#include "BrytecConfigEmbedded/Boards/BrytecBoard.h"
#include "BrytecConfigEmbedded/EBrytecApp.h"
#include "BrytecConfigEmbedded/ENode.h"

void ENodeGroup::setupPin()
{
    BrytecBoard::setupPin(boardPinIndex, type);
}

void ENodeGroup::updateInitialValue()
{
    ENode* node = EBrytecApp::getInitialValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    node->SetValue(0, BrytecBoard::getPinValue(boardPinIndex));
}

void ENodeGroup::updateFinalValue()
{
    ENode* node = EBrytecApp::getFinalValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    float* output = node->GetOutput();
    if (!output)
        return;

    BrytecBoard::setPinValue(boardPinIndex, *output);
}