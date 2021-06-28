/*
 * Copyright (c) 2021 krishpranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/String.h>
#include <AK/StringBuilder.h>
#include <AK/Time.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/times.h>
#include <syscall.h>
#include <time.h>
#include <utime.h>

extern "C" {

time_t time(time_t* tloc)
{
    struct timeval tv;
    struct timezone tz;
    if (gettimeofday(&tv, &tz) < 0)
        return (time_t)-1;
    if (tloc)
        *tloc = tv.tv_sec;
    return tv.tv_sec;
}

int adjtime(const struct timeval* delta, struct timeval* old_delta)
{
    int rc = syscall(SC_adjtime, delta, old_delta);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int gettimeofday(struct timeval* __restrict__ tv, void* __restrict__)
{
    int rc = syscall(SC_gettimeofday, tv);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}


}