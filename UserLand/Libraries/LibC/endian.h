/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN 4321
#define __PDP_ENDIAN 3412

#if defined(__GNUC__) && defined(__BYTE_ORDER__)
#    define __BYTE_ORDER __BYTE_ORDER__
#else
#    include <bits/endian.h>
#endif

#if defined(_GNU_SOURCE) || defined(_BSD_SOURCE)

#   include <stdint.h>


static __inline uint16_t __bswap16(uint16_t x)
{
    return __builtin_bswap16(x);
}

static __inline uint32_t __bswap32(uint32_t x)
{
    return __builtin_bswap32(x);
}

static __inline uint64_t __bswap64(uint64_t x)
{
    return __builtin_bswap64(x);
}