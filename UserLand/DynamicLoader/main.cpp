/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


// includes
#include <LibC/sys/internals.h>
#include <LibC/unistd.h>
#include <LibELF/AuxiliaryVector.h>
#include <LibELF/DynamicLinker.h>

char* __static_environ[] = { nullptr };

static void init_libc()
{
    environ = __static_environ;
    __environ_is_malloced = false;
    __stdio_is_initialized = false;
    __libc_init();
}