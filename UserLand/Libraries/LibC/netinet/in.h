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


#define MCAST_JOIN_SOURCE_GROUP 100
#define MCAST_LEAVE_SOURCE_GROUP 101

#define IPPORT_RESERVED 1024
#define IPPORT_USERRESERVED 5000

typedef uint16_t in_port_t;

struct in_addr {
    uint32_t s_addr;
};

struct sockaddr_in {
    sa_family_t sin_family;
    in_port_t sin_port;
    struct in_addr sin_addr;
    char sin_zero[8];
};

struct ip_mreq {
    struct in_addr imr_multiaddr;
    struct in_addr imr_interface;
};

struct group_source_req {
    uint32_t gsr_interface;
    struct sockaddr_storage gsr_group;
    struct sockaddr_storage gsr_source;
};

struct ip_mreq_source {
    struct in_addr imr_multiaddr;
    struct in_addr imr_sourceaddr;
    struct in_addr imr_interface;
};

#define IPV6_UNICAST_HOPS 1
#define IPV6_MULTICAST_HOPS 2
#define IPV6_MULTICAST_LOOP 3
#define IPV6_MULTICAST_IF 4
#define IPV6_ADD_MEMBERSHIP 5
#define IPV6_DROP_MEMBERSHIP 6
#define IP_ADD_SOURCE_MEMBERSHIP 7
#define IP_DROP_SOURCE_MEMBERSHIP 8
#define IPV6_V6ONLY 9

struct in6_addr {
    uint8_t s6_addr[16];
};

#define IN6ADDR_ANY_INIT                               \
    {                                                  \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
    }

extern struct in6_addr in6addr_any;

struct sockaddr_in6 {
    sa_family_t sin6_family;   
    in_port_t sin6_port;       
    uint32_t sin6_flowinfo;    
    struct in6_addr sin6_addr; 
    uint32_t sin6_scope_id;   
};

struct ipv6_mreq {
    struct in6_addr ipv6mr_multiaddr;
    uint32_t ipv6mr_interface;
};

__END_DECLS
