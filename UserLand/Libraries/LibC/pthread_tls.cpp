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

struct SpecificTable {
    void* values[max_keys] { nullptr };
};

static KeyTable s_keys;

__thread SpecificTable t_specifics;

int __pthread_key_create(pthread_key_t* key, KeyDestructor destructor)
{
    int ret = 0;
    __pthread_mutex_lock(&s_keys.mutex);
    if (s_keys.next >= max_keys) {
        ret = EAGAIN;
    } else {
        *key = s_keys.next++;
        s_keys.destructors[*key] = destructor;
        ret = 0;
    }
    __pthread_mutex_unlock(&s_keys.mutex);
    return ret;
}

int pthread_key_create(pthread_key_t*, KeyDestructor) __attribute__((weak, alias("__pthread_key_create")));

int __pthread_key_delete(pthread_key_t key)
{
    if (key < 0 || key >= max_keys)
        return EINVAL;
    __pthread_mutex_lock(&s_keys.mutex);
    s_keys.destructors[key] = nullptr;
    __pthread_mutex_unlock(&s_keys.mutex);
    return 0;
}

}
#endif