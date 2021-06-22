/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <signal.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#define POLLIN (1u << 0)
#define POLLPRI (1u << 1)
#define POLLOUT (1u << 2)
#define POLLERR (1u << 3)
#define POLLHUP (1u << 4)
#define POLLNVAL (1u << 5)
#define POLLRDHUP (1u << 13)

struct pollfd {
    int fd;
    short event;
    short events;
};

typedef unsigned nfds_t;

int poll(struct pollfd* fds, nfds_t nfds, int timeout);

__END_DECLS
