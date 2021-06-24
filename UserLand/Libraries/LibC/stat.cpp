/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <syscall.h>
#include <unistd.h>

extern "C" {

mode_t umask(mode_t mask) 
{
    return syscall(SC_umask, mask);
}

}