#include "ENode.h"

#define COMPARE_AND_CREATE_NODE(type) \
    case NodeTypes::type:             \
        return CreateNodeInPlace<NodeTypes::type>(spec, destination)

namespace Brytec {

ENode* CreateNode(const ENodeSpec& spec, uint8_t* destination)
{
    if (!destination)
        return nullptr;

    switch (spec.type) {
        COMPARE_AND_CREATE_NODE(Final_Value);
        COMPARE_AND_CREATE_NODE(Initial_Value);
        COMPARE_AND_CREATE_NODE(Node_Group);
        COMPARE_AND_CREATE_NODE(And);
        COMPARE_AND_CREATE_NODE(Or);
        COMPARE_AND_CREATE_NODE(Two_Stage);
        COMPARE_AND_CREATE_NODE(Curve);
        COMPARE_AND_CREATE_NODE(Compare);
        COMPARE_AND_CREATE_NODE(On_Off);
        COMPARE_AND_CREATE_NODE(Invert);
        COMPARE_AND_CREATE_NODE(Toggle);
        COMPARE_AND_CREATE_NODE(Delay);
        COMPARE_AND_CREATE_NODE(Push_Button);
        COMPARE_AND_CREATE_NODE(Map_Value);
        COMPARE_AND_CREATE_NODE(Math);
        COMPARE_AND_CREATE_NODE(Value);
        COMPARE_AND_CREATE_NODE(Switch);
        COMPARE_AND_CREATE_NODE(CanBusInput);
        COMPARE_AND_CREATE_NODE(PinCurrent);
        COMPARE_AND_CREATE_NODE(PID);
        COMPARE_AND_CREATE_NODE(Counter);
        COMPARE_AND_CREATE_NODE(Color);
        COMPARE_AND_CREATE_NODE(Racepak_Switch_Panel);
        COMPARE_AND_CREATE_NODE(Holley_Broadcast);
        COMPARE_AND_CREATE_NODE(Interpolate);
        COMPARE_AND_CREATE_NODE(CanBusOutput);
        COMPARE_AND_CREATE_NODE(Thermistor);
        COMPARE_AND_CREATE_NODE(Holley_Io_Module);
        COMPARE_AND_CREATE_NODE(Mix_Color);
        COMPARE_AND_CREATE_NODE(Clamp);
        COMPARE_AND_CREATE_NODE(DeltaTime);
        COMPARE_AND_CREATE_NODE(PreviousValue);
        COMPARE_AND_CREATE_NODE(Change);
        COMPARE_AND_CREATE_NODE(OneDMap);

    default:
        return nullptr;
    }
}

}