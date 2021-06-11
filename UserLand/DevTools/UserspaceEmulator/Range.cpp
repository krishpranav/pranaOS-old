/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "Range.h"
#include <AK/Vector.h>

namespace UserspaceEmulator {

Vector<Range, 2> Range::carve(const Range& taken) const
{
    VERIFY((taken.size() % PAGE_SIZE) == 0);
    Vector<Range, 2> parts;
}

}