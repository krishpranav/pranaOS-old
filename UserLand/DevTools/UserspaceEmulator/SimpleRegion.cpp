/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include "SimpleRegion.h"
#include <string.h>

namespace UserspaceEmulator {


SimpleRegion::SimpleRegion(u32 base, u32 size)
    : Region(base, size)
{
    m_data = (u8*)calloc(1, size);
    m_shadow_data = (u8*)malloc(size);
    memset(m_shadow_data, 1, size);
}

SimpleRegion::~SimpleRegion()
{
    free(m_shadow_data);
    free(m_data);
}

ValueWithShadow<u8> SimpleRegion::read8(FlatPtr offset)
{
    VERIFY(offset < size());
    return { *reinterpret_cast<const u8*>(m_data + offset), *reinterpret_cast<const u8*>(m_shadow_data + offset) };
}


ValueWithShadow<u16> SimpleRegion::read16(u32 offset)
{
    VERIFY(offset + 1 < size());
    return { *reinterpret_cast<const u16*>(m_data + offset), *reinterpret_cast<const u16*>(m_shadow_data + offset) };
}

}