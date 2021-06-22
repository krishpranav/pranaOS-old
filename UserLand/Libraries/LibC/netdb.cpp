/*
 * Copyright (c) 2021, FiddlePie
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <AK/Assertions.h>
#include <AK/ByteBuffer.h>
#include <AK/ScopeGuard.h>
#include <AK/String.h>
#include <Kernel/Net/IPv4.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

extern "C" {

int h_errno;

static hostent __gethostbyname_buffer;
static in_addr_t __gethostbyname_address;
static in_addr_t* __gethostbyname_address_list_buffer[2];

static hostent __gethostbyaddr_buffer;
static in_addr_t* __gethostbyaddr_address_list_buffer[2];
static constexpr i32 lookup_server_endpoint_magic = 9001;

// Get service entry buffers and file information for the getservent() family of functions.
static FILE* services_file = nullptr;
static const char* services_path = "/etc/services";

static bool fill_getserv_buffers(const char* line, ssize_t read);
static servent __getserv_buffer;
static String __getserv_name_buffer;
static String __getserv_protocol_buffer;
static int __getserv_port_buffer;
static Vector<ByteBuffer> __getserv_alias_list_buffer;
static Vector<char*> __getserv_alias_list;
static bool keep_service_file_open = false;
static ssize_t service_file_offset = 0;

// Get protocol entry buffers and file information for the getprotent() family of functions.
static FILE* protocols_file = nullptr;
static const char* protocols_path = "/etc/protocols";

static bool fill_getproto_buffers(const char* line, ssize_t read);
static protoent __getproto_buffer;
static String __getproto_name_buffer;
static Vector<ByteBuffer> __getproto_alias_list_buffer;
static Vector<char*> __getproto_alias_list;
static int __getproto_protocol_buffer;
static bool keep_protocols_file_open = false;
static ssize_t protocol_file_offset = 0;
