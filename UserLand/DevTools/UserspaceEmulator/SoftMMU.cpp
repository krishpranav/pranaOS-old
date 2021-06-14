/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "SoftMMU.h"
#include "Emulator.h"
#include "MmapRegion.h"
#include "Report.h"
#include <AK/ByteBuffer.h>
#include <AK/Memory.h>
#include <AK/QuickSort.h>

namespace UserspaceEmulator {

SoftMMU::SoftMMU(Emulator& emulator)
    : m_emulator(emulator)
{
}

void SoftMMU::add_region(NonnullOwnPtr<Region> region)
{
    VERIFY(!find_region({ 0x23, region->base() }));

    size_t first_page_in_region = region->base() / PAGE_SIZE;
    size_t last_page_in_region = (region->base() + region->size() - 1) / PAGE_SIZE;
    for (size_t page = first_page_in_region; page <= last_page_in_region; ++page) {
        m_page_to_region_map[page] = region.ptr();
    }

    m_regions.append(move(region));
    quick_sort((Vector<OwnPtr<Region>>&)m_regions, [](auto& a, auto& b) { return a->base() < b->base(); });
}

void SoftMMU::remove_region(Region& region)
{
    size_t first_page_in_region = region.base() / PAGE_SIZE;
    for (size_t i = 0; i < ceil_div(region.size(), PAGE_SIZE); ++i) {
        m_page_to_region_map[first_page_in_region + i] = nullptr;
    }

    m_regions.remove_first_matching([&](auto& entry) { return entry.ptr() == &region; });
}


}