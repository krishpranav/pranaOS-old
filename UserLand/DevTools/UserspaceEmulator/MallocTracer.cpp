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


}