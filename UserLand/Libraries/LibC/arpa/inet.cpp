/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>

extern "C" {

const char* inet_ntop(int af, const void* src, char* dst, socklen_t len)
{
    if (af != AF_INET) {
        errno = EAFNOSUPPORT;
        return nullptr;
    }
    if (len < 4) {
        errno = ENOSPC;
        return nullptr;
    }
    auto* bytes = (const unsigned char*)src;
    snprintf(dst, len, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);
    return (const char*)dst;
}

}