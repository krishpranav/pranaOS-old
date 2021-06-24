/*
 * Copyright (c) 2021 nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Format.h>
#include <assert.h>
#include <errno.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

int kill(pid_t pid, int sig)
{
    int rc = syscall(SC_kill, pid, sig);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int killpg(int pgrp, int sig)
{
    int rc = syscall(SC_killpg, pgrp, sig);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int raise(int sig)
{
    return kill(getpid(), sig);
}

}