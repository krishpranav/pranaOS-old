/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <AK/IntrusiveList.h>
#include <AK/Types.h>

#define MAGIC_PAGE_HEADER 0x42657274     // 'Bert'
#define MAGIC_BIGALLOC_HEADER 0x42697267 // 'Birg'
#define MALLOC_SCRUB_BYTE 0xdc
#define FREE_SCRUB_BYTE 0xed

#define PAGE_ROUND_UP(x) ((((size_t)(x)) + PAGE_SIZE - 1) & (~(PAGE_SIZE - 1)))

static constexpr unsigned short size_classes[] = { 8, 16, 32, 64, 128, 256, 504, 1016, 2032, 4088, 8184, 16376, 32752, 0 };
static constexpr size_t num_size_classes = (sizeof(size_classes) / sizeof(unsigned short)) - 1;

consteval bool check_size_classes_alignment()
{
    for (size_t i = 0; i < num_size_classes; i++) {
        if ((size_classes[i] % 8) != 0)
            return false;
    }
    return true;
}