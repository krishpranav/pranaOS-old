/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "RangeAllocator.h"
#include <AK/BinarySearch.h>
#include <AK/Checked.h>
#include <AK/Random.h>

#define VM_GUARD_PAGES
#define PAGE ((FlatPtr)0xfffff000u)


namespace UserspaceEmulator {

RangeAllocator::RangeAllocator()
    : m_total_range({}, 0)
{
}

void RangeAllocator::initialize_with_range(VirtualAddress base, size_t size)
{
    m_total_range = { base, size };
    m_available_ranges.append({ base, size });
}

void RangeAllocator::dump() const
{
    dbgln("RangeAllocator({})", this);
    for (auto& range : m_available_ranges) {
        dbgln("    {:x} -> {:x}", range.base().get(), range.end().get() - 1);
    }
}

void RangeAllocator::carve_at_index(int index, const Range& range)
{
    auto remaining_parts = m_available_ranges[index].carve(range);
    VERIFY(remaining_parts.size() >= 1);
    VERIFY(m_total_range.contains(remaining_parts[0]));
    m_available_ranges[index] = remaining_parts[0];
    if (remaining_parts.size() == 2) {
        VERIFY(m_total_range.contains(remaining_parts[1]));
        m_available_ranges.insert(index + 1, move(remaining_parts[1]));
    }
}


}