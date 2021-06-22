/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Debug.h>
#include <AK/ScopedValueRollback.h>
#include <AK/Vector.h>
#include <LibELF/AuxiliaryVector.h>
#include <LibThreading/Lock.h>
#include <assert.h>
#include <errno.h>
#include <mallocdefs.h>
#include <serenity.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <sys/mman.h>
#include <syscall.h>

#define RECYCLE_BIG_ALLOCATIONS

static Threading::Lock& malloc_lock()
{
    static u32 lock_storage[sizeof(Threading::Lock) / sizeof(u32)];
    return *reinterpret_cast<Threading::Lock*>(&lock_storage);
}

constexpr size_t number_of_hot_chunked_blocks_to_keep_around = 16;
constexpr size_t number_of_cold_chunked_blocks_to_keep_around = 16;
constexpr size_t number_of_big_blocks_to_keep_around_per_size_class = 8;

static bool s_log_malloc = false;
static bool s_scrub_malloc = true;
static bool s_scrub_free = true;
static bool s_profiling = false;
static bool s_in_userspace_emulator = false;

ALWAYS_INLINE static void ue_notify_malloc(const void* ptr, size_t size)
{
    if (s_in_userspace_emulator)
        syscall(SC_emuctl, 1, size, (FlatPtr)ptr);
}

ALWAYS_INLINE static void ue_notify_free(const void* ptr)
{
    if (s_in_userspace_emulator)
        syscall(SC_emuctl, 2, (FlatPtr)ptr, 0);
}

ALWAYS_INLINE static void ue_notify_realloc(const void* ptr, size_t size)
{
    if (s_in_userspace_emulator)
        syscall(SC_emuctl, 3, size, (FlatPtr)ptr);
}

ALWAYS_INLINE static void ue_notify_chunk_size_changed(const void* block, size_t chunk_size)
{
    if (s_in_userspace_emulator)
        syscall(SC_emuctl, 4, chunk_size, (FlatPtr)block);
}

struct MallocStats {
    size_t number_of_malloc_calls;

    size_t number_of_big_allocator_hits;
    size_t number_of_big_allocator_purge_hits;
    size_t number_of_big_allocs;

    size_t number_of_hot_empty_block_hits;
    size_t number_of_cold_empty_block_hits;
    size_t number_of_cold_empty_block_purge_hits;
    size_t number_of_block_allocs;
    size_t number_of_blocks_full;

    size_t number_of_free_calls;

    size_t number_of_big_allocator_keeps;
    size_t number_of_big_allocator_frees;

    size_t number_of_freed_full_blocks;
    size_t number_of_hot_keeps;
    size_t number_of_cold_keeps;
    size_t number_of_frees;
};
static MallocStats g_malloc_stats = {};

static size_t s_hot_empty_block_count { 0 };
static ChunkedBlock* s_hot_empty_blocks[number_of_hot_chunked_blocks_to_keep_around] { nullptr };
static size_t s_cold_empty_block_count { 0 };
static ChunkedBlock* s_cold_empty_blocks[number_of_cold_chunked_blocks_to_keep_around] { nullptr };

struct Allocator {
    size_t size { 0 };
    size_t block_count { 0 };
    ChunkedBlock::List usable_blocks;
    ChunkedBlock::List full_blocks;
};

struct BigAllocator {
    Vector<BigAllocationBlock*, number_of_big_blocks_to_keep_around_per_size_class> blocks;
};

static u8 g_allocators_storage[sizeof(Allocator) * num_size_classes];
static u8 g_big_allocators_storage[sizeof(BigAllocator)];

static inline Allocator (&allocators())[num_size_classes]
{
    return reinterpret_cast<Allocator(&)[num_size_classes]>(g_allocators_storage);
}

static inline BigAllocator (&big_allocators())[1]
{
    return reinterpret_cast<BigAllocator(&)[1]>(g_big_allocators_storage);
}