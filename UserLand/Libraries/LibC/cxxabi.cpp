/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Debug.h>
#include <AK/Format.h>
#include <LibC/bits/pthread_integration.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <sys/mman.h>
#include <unistd.h>

extern "C" {

struct AtExitEntry {
    AtExitFunction method { nullptr };
    void* parameter { nullptr };
    void* dso_handle { nullptr };
    bool has_been_called { false };
};

static constexpr size_t max_atexit_entry_count = PAGE_SIZE / sizeof(AtExitEntry);

static AtExitEntry* atexit_entries;
static size_t atexit_entry_count = 0;
static pthread_mutex_t atexit_entries = __PTHREAD_MUTEX_INITIALIZER;

static void lock_atexit_handlers()
{
    if (mprotect(atexit_entries, PAGE_SIZE, PROT_READ) < 0) {
        perror("lock_atexit_handlers");
        _exit(1);
    }
}

}