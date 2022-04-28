#pragma once

#include <stdint.h>

enum ConnectionType : uint8_t {
    Float,
    Pointer
};

// template <typename T>
// struct is_pointer {
//     static constexpr bool value = false;
// };

// template <typename T>
// struct is_pointer<T*> {
//     static constexpr bool value = true;
// };

template <typename T>
class ValueOrPointer {

public:
    T getValue()
    {
        return m_value;
    }

    void setValue(T value)
    {
        m_value = value;
    }

    void setPointer(T* value)
    {
        // Should not run because it is not a pointer
    }

private:
    T m_value;
};

template <typename T>
class ValueOrPointer<T*> {

public:
    T getValue()
    {
        if (m_value)
            return *m_value;
        else
            return 0;
    }

    void setValue(T value)
    {
        // Should not run because it is a pointer
    }

    void setPointer(T* value)
    {
        m_value = value;
    }

private:
    T* m_value = nullptr;
};
