/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <errno.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <syscall.h>

extern "C" {


int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout_tv)
{
    timespec* timeout_ts = nullptr;
    timespec timeout;
    if (timeout_tv) {
        timeout_ts = &timeout;
        TIMEVAL_TO_TIMESPEC(timeout_tv, timeout_ts);
    }
    return pselect(nfds, readfds, writefds, exceptfds, timeout_ts, nullptr);
}

}