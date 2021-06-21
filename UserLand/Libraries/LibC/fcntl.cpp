/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <syscall.h>

extern "C" {

int fcntl(int fd, int cmd, ...)
{
    va_list ap;
    va_start(ap, cmd);
    u32 extra_arg = va_arg(ap, u32);
    int rc = syscall(SC_fcntl, fd, cmd, extra_arg);
    va_end(ap);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}