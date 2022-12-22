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

    if (tripped)
        return;

    ENode* node = EBrytecApp::getFinalValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    float* output = node->GetOutput();
    if (!output)
        return;

    // Clamp to 0.0 - 1.0
    if (*output > 1.0f)
        *output = 1.0f;
    if (*output < 0.0f)
        *output = 0.0f;

    BrytecBoard::setPinValue(boardPinIndex, type, *output);
}

void ENodeGroup::updatePinCurrent(float timestep)
{
    if (!enabled)
        return;

    float current = BrytecBoard::getPinCurrent(boardPinIndex);
    checkOverCurrent(timestep, current);

    ENode* node
        = EBrytecApp::getPinCurrentNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    node->SetValue(0, current);
}

void ENodeGroup::checkOverCurrent(float timestep, float current)
{
    if (tripped) {
        if (numberRetries >= maxRetries) {
            return;
        }

        retryTimer += timestep;

        if (retryTimer >= retryDelay) {
            tripped = false;
            retryTimer = 0.0f;
            numberRetries += 1;
            return;
        }
    }

    if (current >= currentLimit)
        trippedTimer += timestep;

    if (trippedTimer >= TrippedTime) {
        BrytecBoard::setPinValue(boardPinIndex, type, 0.0f);
        tripped = true;
        trippedTimer = 0.0f;
    }
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