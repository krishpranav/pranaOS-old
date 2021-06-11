/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "MallocTracer.h"
#include "Emulator.h"
#include "MmapRegion.h"
#include <AK/Debug.h>
#include <AK/TemporaryChange.h>
#include <mallocdefs.h>
#include <string.h>
#include <unistd.h>

namespace UserspaceEmulator {


MallocTracer::MallocTracer(Emulator& emulator) 
    : m_emulator(emulator)
{
}

template<typename Callback>
inline void MallocTracer::for_each_mallocation(Callback callback) const
{
    m_emulator.mmu().for_each_region([&](auto& region) {
        if (is<MmapRegion>(region) && static_cast<const MmapRegion&>(region).is_malloc_block()) {
            auto* malloc_data = static_cast<MmapRegion&>(region).malloc_metadata();
            for (auto& mallocation : malloc_data->mallocations) {
                if (mallocation.used && callback(mallocation) == IterationDecision::Break)
                    return IterationDecision::Break;
            }
        }
        return IterationDecision::Continue;
    });
}


void MallocTracer::update_metadata(MmapRegion& mmap_region, size_t chunk_size)
{
    mmap_region.set_malloc_metadata({},
        adopt_own(*new MallocRegionMetadata {
            .region = mmap_region,
            .address = mmap_region.base(),
            .chunk_size = chunk_size,
            .mallocations = {},
        }));
    auto& malloc_data = *mmap_region.malloc_metadata();

    bool is_chunked_block = malloc_data.chunk_size <= size_classes[num_size_classes - 1];
    if (is_chunked_block)
        malloc_data.mallocations.resize((ChunkedBlock::block_size - sizeof(ChunkedBlock)) / malloc_data.chunk_size);
    else
        malloc_data.mallocations.resize(1);

    mmap_region.set_malloc(true);
}

void MallocTracer::target_did_malloc(Badge<Emulator>, FlatPtr address, size_t size)
{
    if (m_emulator.is_in_loader_code())
        return;
    auto* region = m_emulator.mmu().find_region({ 0x23, address });
    VERIFY(region);
    VERIFY(is<MmapRegion>(*region));
    auto& mmap_region = static_cast<MmapRegion&>(*region);

    auto* shadow_bits = mmap_region.shadow_data() + address - mmap_region.base();
    memset(shadow_bits, 0, size);

    if (auto* existing_mallocation = find_mallocation(address)) {
        VERIFY(existing_mallocation->freed);
        existing_mallocation->size = size;
        existing_mallocation->freed = false;
        existing_mallocation->malloc_backtrace = m_emulator.raw_backtrace();
        existing_mallocation->free_backtrace.clear();
        return;
    }

    if (!mmap_region.is_malloc_block()) {
        auto chunk_size = mmap_region.read32(offsetof(CommonHeader, m_size)).value();
        update_metadata(mmap_region, chunk_size);
    }
    auto* mallocation = mmap_region.malloc_metadata()->mallocation_for_address(address);
    VERIFY(mallocation);
    *mallocation = { address, size, true, false, m_emulator.raw_backtrace(), Vector<FlatPtr>() };
}

}