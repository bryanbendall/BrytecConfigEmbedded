#pragma once

#include <stddef.h>

struct IOTypes {

    enum class Types {
        Undefined,

        Output_5V,
        Output_Batt,
        Output_Ground,

        Input_Batt,
        Input_Ground,
        Input_5V,
        Input_5V_Variable,
        Input_20V_Variable,

        Internal,

        Count
    };

    static const char* Strings[(size_t)Types::Count];

    static const char* getString(Types type) { return Strings[(int)type]; }
};
