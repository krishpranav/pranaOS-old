/*
 * Copyright (c) 2021, Krisna Pranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/types.h

__BEGIN_DECLS

int sched_yield();

struct sched_param {
    int sched_priority;
};

#define SCHED_FIFO 0
#define SCHED_RR 1
#define SCHED_OTHER 2
#define SCHED_BATCH 3

__END_DECLS