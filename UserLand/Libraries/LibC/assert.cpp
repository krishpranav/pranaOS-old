/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Format.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

extern bool __stdio_is_initialized;
#ifdef DEBUG
void __assertion_failed(const char* msg)
{
    dbgln("USERSPACE({}) ASSERTION FAILED: {}", getpid(), msg);
    if (__stdio_is_initialized)
        warnln("ASSERTION FAILED: {}", msg);

    Syscall::SC_set_coredump_metadata_params params {
        { "assertion", strlen("assertion") },
        { msg, strlen(msg) },
    };
    syscall(SC_set_coredump_metadata, &params);
    abort();
}
#endif
}

void _abort()
{
    asm volatile("ud2");
    __builtin_unreachable();
}
