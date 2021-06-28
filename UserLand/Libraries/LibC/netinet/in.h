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

__END_DECLS