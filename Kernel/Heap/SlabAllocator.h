#pragma once

// includes
#include <AK/Function.h>
#include <AK/Types.h>

namespace Kernel {

#define SLAB_ALLOC_SCRUB_BYTE 0xab
#define SLAB_DEALLOC_SCRUB_BYTE 0xbc

void* slab_alloc(size_t slab_size);
void slab_dealloc(void*, size_t slab_size);
void slab_alloc_init();
void slab_alloc_stats(Function<void(size_t slab_size, size_t allocated, size_t free)>);

#define MAKE_SLAB_ALLOCATED(type)                                        \
public:                                                                  \
    void* operator new(size_t) { return slab_alloc(sizeof(type)); }      \
    void operator delete(void* ptr) { slab_dealloc(ptr, sizeof(type)); } \
                                                                         \
private:

}