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

}