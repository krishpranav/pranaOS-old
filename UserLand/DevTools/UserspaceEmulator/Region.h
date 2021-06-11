/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes 
#include "Range.h"
#include "ValueWithShadow.h"
#include <AK/TypeCasts.h>
#include <AK/Types.h>
#include <LibX86/Types.h>

namespace UserspaceEmulator {

class Emulator;

class Region {
public:
    virtual ~Region() { }

    const Range& range() const { return m_range; }

    u32 base() const { return m_range.base().get(); }
    u32 size() const { return m_range.size(); }
    u32 end() const { return m_range.end().get(); }
}

}
