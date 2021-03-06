/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/socket.h>

struct rtentry {
    struct sockaddr rt_gateway;
    struct sockaddr rt_genmask;
    unsigned short int rt_flags;
    char* rt_dev;
};

#define RTF_UP 0x1  
#define RTF_GATEWAY 0x2 
