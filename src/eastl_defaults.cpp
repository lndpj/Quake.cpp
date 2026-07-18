#include <new>
#include <cstdlib>
#include <cstddef>
#include <cassert>

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
#endif

// 1. Standard Allocation Overload
// Used by EASTL for regular allocations.
void* operator new[](size_t size, const char* /*pName*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) {
    void* ptr = std::malloc(size);
    assert(ptr != nullptr && "Out of memory");
    return ptr;
}

// 2. Aligned Allocation Overload
// Used by EASTL for allocations requiring specific memory alignment.
void* operator new[](size_t size, size_t alignment, size_t /*alignmentOffset*/, const char* /*pName*/, int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) {
#if defined(_MSC_VER) || defined(__MINGW32__)
    void* ptr = _aligned_malloc(size, alignment);
#else
    // posix_memalign requires alignment to be at least sizeof(void*) and a power of two.
    if (alignment < sizeof(void*)) {
        alignment = sizeof(void*);
    }
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        ptr = nullptr;
    }
#endif

    assert(ptr != nullptr && "Out of memory");
    return ptr;
}
