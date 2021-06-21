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

int create_inode_watcher(unsigned flags)
{
    int rc = syscall(SC_create_inode_watcher, flags);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

int inode_watcher_add_watch(int fd, const char* path, size_t path_length, unsigned event_mask)
{
    Syscall::SC_inode_watcher_add_watch_params params { fd, { path, path_length }, event_mask };
    int rc = syscall(SC_inode_watcher_add_watch, &params);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}