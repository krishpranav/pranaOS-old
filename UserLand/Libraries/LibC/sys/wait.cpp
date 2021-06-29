/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <errno.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>


extern "C" {

pid_t wait(int* wstatus)
{
    return waitpid(-1, wstatus, 0);
}
}