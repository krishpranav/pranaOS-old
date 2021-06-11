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

}