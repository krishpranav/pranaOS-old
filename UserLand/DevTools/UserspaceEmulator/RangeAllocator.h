/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include "Range.h"
#include <AK/Vector.h>

namespace UserspaceEmulator {

class RangeAllocator {
public:
    RangeAllocator();

    void initialize_with_range(VirtualAddress, size_t);

    Optional<Range> allocate_anywhere(size_t, size_t alignment = PAGE_SIZE);
    Optional<Range> allocate_specific(VirtualAddress, size_t);
    Optional<Range> allocate_randomized(size_t, size_t alignment);
    
}
}