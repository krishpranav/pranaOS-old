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

static Allocator* allocator_for_size(size_t size, size_t& good_size)
{
    for (size_t i = 0; size_classes[i]; ++i) {
        if (size <= size_classes[i]) {
            good_size = size_classes[i];
            return &allocators()[i];
        }
    }
    good_size = PAGE_ROUND_UP(size);
    return nullptr;
}

#ifdef RECYCLE_BIG_ALLOCATIONS
static BigAllocator* big_allocator_for_size(size_t size)
{
    if (size == 65536)
        return &big_allocators()[0];
    return nullptr;
}
#endif

extern "C" {

static void* os_alloc(size_t size, const char* name)
{
    auto* ptr = serenity_mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0, ChunkedBlock::block_size, name);
    VERIFY(ptr != MAP_FAILED);
    return ptr;
}

static void os_free(void* ptr, size_t size)
{
    int rc = munmap(ptr, size);
    assert(rc == 0);
}

enum class CallerWillInitializeMemory {
    No,
    Yes,
};

static void* malloc_impl(size_t size, CallerWillInitializeMemory caller_will_initialize_memory)
{
    Threading::Locker locker(malloc_lock());

    if (s_log_malloc)
        dbgln("LibC: malloc({})", size);

    if (!size) {
        // Legally we could just return a null pointer here, but this is more
        // compatible with existing software.
        size = 1;
    }

    g_malloc_stats.number_of_malloc_calls++;

    size_t good_size;
    auto* allocator = allocator_for_size(size, good_size);

    if (!allocator) {
        size_t real_size = round_up_to_power_of_two(sizeof(BigAllocationBlock) + size, ChunkedBlock::block_size);
#ifdef RECYCLE_BIG_ALLOCATIONS
        if (auto* allocator = big_allocator_for_size(real_size)) {
            if (!allocator->blocks.is_empty()) {
                g_malloc_stats.number_of_big_allocator_hits++;
                auto* block = allocator->blocks.take_last();
                int rc = madvise(block, real_size, MADV_SET_NONVOLATILE);
                bool this_block_was_purged = rc == 1;
                if (rc < 0) {
                    perror("madvise");
                    VERIFY_NOT_REACHED();
                }
                if (mprotect(block, real_size, PROT_READ | PROT_WRITE) < 0) {
                    perror("mprotect");
                    VERIFY_NOT_REACHED();
                }
                if (this_block_was_purged) {
                    g_malloc_stats.number_of_big_allocator_purge_hits++;
                    new (block) BigAllocationBlock(real_size);
                }

                ue_notify_malloc(&block->m_slot[0], size);
                return &block->m_slot[0];
            }
        }
#endif
        g_malloc_stats.number_of_big_allocs++;
        auto* block = (BigAllocationBlock*)os_alloc(real_size, "malloc: BigAllocationBlock");
        new (block) BigAllocationBlock(real_size);
        ue_notify_malloc(&block->m_slot[0], size);
        return &block->m_slot[0];
    }

    ChunkedBlock* block = nullptr;
    for (auto& current : allocator->usable_blocks) {
        if (current.free_chunks()) {
            block = &current;
            break;
        }
    }

    if (!block && s_hot_empty_block_count) {
        g_malloc_stats.number_of_hot_empty_block_hits++;
        block = s_hot_empty_blocks[--s_hot_empty_block_count];
        if (block->m_size != good_size) {
            new (block) ChunkedBlock(good_size);
            ue_notify_chunk_size_changed(block, good_size);
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "malloc: ChunkedBlock(%zu)", good_size);
            set_mmap_name(block, ChunkedBlock::block_size, buffer);
        }
        allocator->usable_blocks.append(*block);
    }

    if (!block && s_cold_empty_block_count) {
        g_malloc_stats.number_of_cold_empty_block_hits++;
        block = s_cold_empty_blocks[--s_cold_empty_block_count];
        int rc = madvise(block, ChunkedBlock::block_size, MADV_SET_NONVOLATILE);
        bool this_block_was_purged = rc == 1;
        if (rc < 0) {
            perror("madvise");
            VERIFY_NOT_REACHED();
        }
        rc = mprotect(block, ChunkedBlock::block_size, PROT_READ | PROT_WRITE);
        if (rc < 0) {
            perror("mprotect");
            VERIFY_NOT_REACHED();
        }
        if (this_block_was_purged || block->m_size != good_size) {
            if (this_block_was_purged)
                g_malloc_stats.number_of_cold_empty_block_purge_hits++;
            new (block) ChunkedBlock(good_size);
            ue_notify_chunk_size_changed(block, good_size);
        }
        allocator->usable_blocks.append(*block);
    }

    if (!block) {
        g_malloc_stats.number_of_block_allocs++;
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "malloc: ChunkedBlock(%zu)", good_size);
        block = (ChunkedBlock*)os_alloc(ChunkedBlock::block_size, buffer);
        new (block) ChunkedBlock(good_size);
        allocator->usable_blocks.append(*block);
        ++allocator->block_count;
    }

    --block->m_free_chunks;
    void* ptr = block->m_freelist;
    if (ptr) {
        block->m_freelist = block->m_freelist->next;
    } else {
        ptr = block->m_slot + block->m_next_lazy_freelist_index * block->m_size;
        block->m_next_lazy_freelist_index++;
    }
    VERIFY(ptr);
    if (block->is_full()) {
        g_malloc_stats.number_of_blocks_full++;
        dbgln_if(MALLOC_DEBUG, "Block {:p} is now full in size class {}", block, good_size);
        allocator->usable_blocks.remove(*block);
        allocator->full_blocks.append(*block);
    }
    dbgln_if(MALLOC_DEBUG, "LibC: allocated {:p} (chunk in block {:p}, size {})", ptr, block, block->bytes_per_chunk());

    if (s_scrub_malloc && caller_will_initialize_memory == CallerWillInitializeMemory::No)
        memset(ptr, MALLOC_SCRUB_BYTE, block->m_size);

    ue_notify_malloc(ptr, size);
    return ptr;
}

}