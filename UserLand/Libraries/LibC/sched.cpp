/*
 * Copyright (c) 2021, Krisna Pranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <errno.h>
#include <sched.h>
#include <syscall.h>

extern "C" {

int sched_yield()
{
    int rc = syscall(SC_yield);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int sched_get_priority_min([[maybe_unused]] int policy)
{
    return 0; 
}

int sched_get_priority_max([[maybe_unused]] int policy)
{
    return 3; 
}

}