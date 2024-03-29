#include "ENodeGroup.h"

#include "Boards/BrytecBoard.h"
#include "EBrytecApp.h"
#include "ENode.h"

namespace Brytec {

void ENodeGroup::setupPin()
{
    if (!enabled)
        return;

    BrytecBoard::setupPin(index, type);
}

void ENodeGroup::updateInitialValue()
{
    if (!enabled)
        return;

    ENode* node = EBrytecApp::getInitialValueNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    node->SetValue(0, BrytecBoard::getPinValue(index, type));
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

    BrytecBoard::setPinValue(index, type, *output);
}

void ENodeGroup::updatePinCurrent(float timestep)
{
    if (!enabled)
        return;

    float current = BrytecBoard::getPinCurrent(index);
    checkOverCurrent(timestep, current);

    ENode* node
        = EBrytecApp::getPinCurrentNode(startNodeIndex, nodeCount);
    if (!node)
        return;

    node->SetValue(0, current);
}

void ENodeGroup::checkOverCurrent(float timestep, float current)
{
    // Skip if current limit not supported
    if (currentLimit == 0)
        return;

    if (tripped) {

        if (!alwaysRetry) {
            // No retry
            if (maxRetries == 0)
                return;

            // Max retries reached
            if (numberRetries >= maxRetries)
                return;
        }

        // Check retry timer
        retryTimer += timestep;
        if (retryTimer >= retryDelay) {
            tripped = false;
            retryTimer = 0.0f;
            numberRetries += 1;
            return;
        }

    } else {

        // Slow blow current limit
        if (current >= (float)currentLimit)
            trippedTimer += timestep;
        else
            trippedTimer -= timestep;

        // Clamp timer to min of 0
        if (trippedTimer < 0.0f)
            trippedTimer = 0.0f;

        // Shut down pin
        if (trippedTimer >= TrippedTime) {
            BrytecBoard::setPinValue(index, type, 0.0f);
            tripped = true;
            trippedTimer = 0.0f;
        }
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

}