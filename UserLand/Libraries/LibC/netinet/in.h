/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <bits/stdint.h>
#include <sys/cdefs.h>
#include <sys/socket.h>


__BEGIN_DECLS

typedef uint32_t in_addr_t;
in_addr_t inet_addr(const char*);

#define INADDR_ANY ((in_addr_t)0)
#define INADDR_NONE ((in_addr_t)-1)
#define INADDR_LOOPBACK 0x7f000001

#define IN_LOOPBACKNET 127

#define IP_TTL 2
#define IP_MULTICAST_LOOP 3
#define IP_ADD_MEMBERSHIP 4
#define IP_DROP_MEMBERSHIP 5
#define IP_MULTICAST_IF 6
#define IP_MULTICAST_TTL 7

__END_DECLS