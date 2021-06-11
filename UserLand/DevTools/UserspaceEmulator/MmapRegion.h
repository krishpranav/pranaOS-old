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

    virtual ValueWithShadow<u8> read8(u32 offset) override;
    virtual ValueWithShadow<u16> read16(u32 offset) override;
    virtual ValueWithShadow<u32> read32(u32 offset) override;
    virtual ValueWithShadow<u64> read64(u32 offset) override;
    virtual ValueWithShadow<u128> read128(u32 offset) override;
    virtual ValueWithShadow<u256> read256(u32 offset) override;

    virtual void write8(u32 offset, ValueWithShadow<u8>) override;
    virtual void write16(u32 offset, ValueWithShadow<u16>) override;
    virtual void write32(u32 offset, ValueWithShadow<u32>) override;
    virtual void write64(u32 offset, ValueWithShadow<u64>) override;
    virtual void write128(u32 offset, ValueWithShadow<u128>) override;
    virtual void write256(u32 offset, ValueWithShadow<u256>) override;

    virtual u8* data() override { return m_data; }
    virtual u8* shadow_data() override { return m_shadow_data; }

    bool is_malloc_block() const { return m_malloc; }
    void set_malloc(bool b) { m_malloc = b; }

    NonnullOwnPtr<MmapRegion> split_at(VirtualAddress);
    

}

}