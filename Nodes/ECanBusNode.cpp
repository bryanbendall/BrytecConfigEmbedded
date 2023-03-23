#include "ECanBusNode.h"

namespace Brytec {

void ECanBusNode::SetInput(uint8_t index, float* output)
{
}

void ECanBusNode::SetValue(uint8_t index, float value)
{
}

float* ECanBusNode::GetOutput(uint8_t index)
{
    return &id;
}

void ECanBusNode::Evaluate(float timestep)
{
}

}