/*
 * Copyright (c) 2021 nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <signal_numbers.h>
#include <sys/types.h>

__BEGIN_DECLS


typedef void (*__sighandler_t)(int);
typedef __sighandler_t sighandler_t;

typedef uint32_t sigset_t;
typedef uint32_t sig_atomic_t;

union sigval {
    int sival_int;
    void* sival_ptr;
};

__END_DECLS