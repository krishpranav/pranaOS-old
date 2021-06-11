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

