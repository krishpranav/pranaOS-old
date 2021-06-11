/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// includes
#include "SoftMMU.h"
#include <sys/mman.h>

namespace UserspaceEmulator {

class MallocRegionMetadata;
class MallocTracer;

class MmapRegion final : public Region {
public:
    static NonnullOwnPtr<MmapRegion> create_anonymous(u32 base, u32 size, u32 prot, String name);
    static NonnullOwnPtr<MmapRegion> create_file_backed(u32 base, u32 size, u32 prot, int flags, int fd, off_t offset, String name);
    virtual ~MmapRegion() override;


}

}