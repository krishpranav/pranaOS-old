/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "MmapRegion.h"
#include "Emulator.h"
#include <string.h>
#include <sys/mman.h>

namespace UserspaceEmulator {

static void* mmap_initialized(size_t bytes, char initial_value, const char* name)
{
    auto* ptr = mmap_with_name(nullptr, bytes, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0, name);
    VERIFY(ptr != MAP_FAILED);
    memset(ptr, initial_value, bytes);
    return ptr;
}

}