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

static void free_pages(void* ptr, size_t bytes)
{
    int rc = munmap(ptr, bytes);
    VERIFY(rc == 0);
}


NonnullOwnPtr<MmapRegion> MmapRegion::create_anonymous(u32 base, u32 size, u32 prot, String name)
{
    auto data = (u8*)mmap_initialized(size, 0, nullptr);
    auto shadow_data = (u8*)mmap_initialized(size, 1, "MmapRegion ShadowData");
    auto region = adopt_own(*new MmapRegion(base, size, prot, data, shadow_data));
    region->m_name = move(name);
    return region;
}

NonnullOwnPtr<MmapRegion> MmapRegion::create_file_backed(u32 base, u32 size, u32 prot, int flags, int fd, off_t offset, String name)
{
    auto real_flags = flags & ~MAP_FIXED;
    auto data = (u8*)mmap_with_name(nullptr, size, prot, real_flags, fd, offset, name.is_empty() ? nullptr : name.characters());
    VERIFY(data != MAP_FAILED);
    auto shadow_data = (u8*)mmap_initialized(size, 1, "MmapRegion ShadowData");
    auto region = adopt_own(*new MmapRegion(base, size, prot, data, shadow_data));
    region->m_file_backed = true;
    region->m_name = move(name);
    return region;
}

MmapRegion::MmapRegion(u32 base, u32 size, int prot, u8* data, u8* shadow_data)
    : Region(base, size, true)
    , m_data(data)
    , m_shadow_data(shadow_data)
{
    set_prot(prot);
}

}