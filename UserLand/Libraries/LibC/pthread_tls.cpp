/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Atomic.h>
#include <LibPthread/pthread.h>
#include <errno.h>
#include <unistd.h>

#ifndef _DYNAMIC_LOADER
extern "C" {

static constexpr int max_keys = PTHREAD_KEYS_MAX;

struct KeyTable {
    KeyDestructor destructors[max_keys] { nullptr };
    int next { 0 };
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};

}