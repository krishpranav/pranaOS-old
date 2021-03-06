/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Types.h>
#include <assert.h>
#include <sys/internals.h>
#include <unistd.h>

extern "C" {

#ifdef NO_TLS
int errno;
#else
__thread int errno;
#endif
char** environ;
bool __environ_is_malloced;
bool __stdio_is_initialized;

void __libc_init()
{
    __malloc_init();
    __stdio_init();
}
}
