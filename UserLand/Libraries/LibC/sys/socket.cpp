/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <syscall.h>

extern "C" {

int socket(int domain, int type, int protocol)
{
    int rc = syscall(SC_socket, domain, type, protocol);
    __RETURN_WITH_ERRNO(rc, rc, -1);
}

}