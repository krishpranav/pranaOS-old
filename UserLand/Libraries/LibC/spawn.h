/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sched.h>
#include <signal.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

enum {
    POSIX_SPAWN_RESETIDS = 1 << 0,
    POSIX_SPAWN_SETPGROUP = 1 << 1,

    POSIX_SPAWN_SETSCHEDPARAM = 1 << 2,
    POSIX_SPAWN_SETSCHEDULER = 1 << 3,

    POSIX_SPAWN_SETSIGDEF = 1 << 4,
    POSIX_SPAWN_SETSIGMASK = 1 << 5,

    POSIX_SPAWN_SETSID = 1 << 6,
};


__END_DECLS