/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <errno.h>
#include <sys/uio.h>
#include <syscall.h>

extern "C" {

ssize_t writev(int fd, const struct iovec* iov, int iov_count)
{
    int rc = syscall(SC_writev, fd, iov, iov_count);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

ssize_t readv(int fd, const struct iovec* iov, int iov_count)
{
    int rc = syscall(SC_readv, fd, iov, iov_count);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}
}
