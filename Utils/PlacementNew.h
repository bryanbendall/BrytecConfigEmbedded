#pragma once

#if __has_include(<new>)
#include <new>
#else

#include <stdlib.h>
void* operator new(size_t size, void* ptr) noexcept;

#endif

