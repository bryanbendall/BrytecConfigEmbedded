#include "PlacementNew.h"

#if !__has_include(<new>)
void* operator new(size_t size, void* ptr) noexcept
{
    (void)size;
    return ptr;
}
#endif