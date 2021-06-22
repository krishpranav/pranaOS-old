/*
 * Copyright (c) 2021, FiddlePie, nuke123-sudo
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

static int connect_to_lookup_server()
{
    int fd = socket(AF_LOCAL, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (fd < 0) {
        perror("socket");
        return -1;
    }

    sockaddr_un address {
        AF_LOCAL,
        "/tmp/portal/lookup"
    };

    if (connect(fd, (const sockaddr*)&address, sizeof(address)) < 0) {
        perror("connect_to_lookup_server");
        close(fd);
        return -1;
    }
    return fd;
}

static String gethostbyname_name_buffer;

hostent* gethostbyname(const char* name)
{
    auto ipv4_address = IPv4Address::from_string(name);

    if (ipv4_address.has_value()) {
        gethostbyname_name_buffer = ipv4_address.value().to_string();
        __gethostbyname_buffer.h_name = const_cast<char*>(gethostbyname_name_buffer.characters());
        __gethostbyname_buffer.h_aliases = nullptr;
        __gethostbyname_buffer.h_addrtype = AF_INET;
        new (&__gethostbyname_address) IPv4Address(ipv4_address.value());
        __gethostbyname_address_list_buffer[0] = &__gethostbyname_address;
        __gethostbyname_address_list_buffer[1] = nullptr;
        __gethostbyname_buffer.h_addr_list = (char**)__gethostbyname_address_list_buffer;
        __gethostbyname_buffer.h_length = 4;

        return &__gethostbyname_buffer;
    }

    int fd = connect_to_lookup_server();
    if (fd < 0)
        return nullptr;

    auto close_fd_on_exit = ScopeGuard([fd] {
        close(fd);
    });

    size_t name_length = strlen(name);

    struct [[gnu::packed]] {
        u32 message_size;
        i32 endpoint_magic;
        i32 message_id;
        i32 name_length;
    } request_header = {
        (u32)(sizeof(request_header) - sizeof(request_header.message_size) + name_length),
        lookup_server_endpoint_magic,
        1,
        (i32)name_length,
    };
    int nsent = write(fd, &request_header, sizeof(request_header));
    if (nsent < 0) {
        perror("write");
        return nullptr;
    }
    VERIFY((size_t)nsent == sizeof(request_header));
    nsent = write(fd, name, name_length);
    if (nsent < 0) {
        perror("write");
        return nullptr;
    }
    VERIFY((size_t)nsent == name_length);

    struct [[gnu::packed]] {
        u32 message_size;
        i32 endpoint_magic;
        i32 message_id;
        i32 code;
        u64 addresses_count;
    } response_header;

    int nrecv = read(fd, &response_header, sizeof(response_header));
    if (nrecv < 0) {
        perror("recv");
        return nullptr;
    }
    VERIFY((size_t)nrecv == sizeof(response_header));
    if (response_header.endpoint_magic != lookup_server_endpoint_magic || response_header.message_id != 2) {
        dbgln("Received an unexpected message");
        return nullptr;
    }
    if (response_header.code != 0) {
        // TODO: return a specific error.
        return nullptr;
    }
    VERIFY(response_header.addresses_count > 0);

    i32 response_length;
    nrecv = read(fd, &response_length, sizeof(response_length));
    if (nrecv < 0) {
        perror("recv");
        return nullptr;
    }
    VERIFY((size_t)nrecv == sizeof(response_length));
    VERIFY(response_length == sizeof(__gethostbyname_address));

    nrecv = read(fd, &__gethostbyname_address, response_length);
    if (nrecv < 0) {
        perror("recv");
        return nullptr;
    }
    VERIFY(nrecv == response_length);

    gethostbyname_name_buffer = name;
    __gethostbyname_buffer.h_name = const_cast<char*>(gethostbyname_name_buffer.characters());
    __gethostbyname_buffer.h_aliases = nullptr;
    __gethostbyname_buffer.h_addrtype = AF_INET;
    __gethostbyname_address_list_buffer[0] = &__gethostbyname_address;
    __gethostbyname_address_list_buffer[1] = nullptr;
    __gethostbyname_buffer.h_addr_list = (char**)__gethostbyname_address_list_buffer;
    __gethostbyname_buffer.h_length = 4;

    return &__gethostbyname_buffer;
}

struct servent* getservent()
{
    if (!services_file) {
        services_file = fopen(services_path, "r");

        if (!services_file) {
            perror("error opening services file");
            return nullptr;
        }
    }

    if (fseek(services_file, service_file_offset, SEEK_SET) != 0) {
        perror("error seeking file");
        fclose(services_file);
        return nullptr;
    }
    char* line = nullptr;
    size_t len = 0;
    ssize_t read;

    auto free_line_on_exit = ScopeGuard([line] {
        if (line) {
            free(line);
        }
    });

    do {
        read = getline(&line, &len, services_file);
        service_file_offset += read;
        if (read > 0 && (line[0] >= 65 && line[0] <= 122)) {
            break;
        }
    } while (read != -1);
    if (read == -1) {
        fclose(services_file);
        services_file = nullptr;
        service_file_offset = 0;
        return nullptr;
    }

    servent* service_entry = nullptr;
    if (!fill_getserv_buffers(line, read))
        return nullptr;

    __getserv_buffer.s_name = const_cast<char*>(__getserv_name_buffer.characters());
    __getserv_buffer.s_port = htons(__getserv_port_buffer);
    __getserv_buffer.s_proto = const_cast<char*>(__getserv_protocol_buffer.characters());

    __getserv_alias_list.clear_with_capacity();
    __getserv_alias_list.ensure_capacity(__getserv_alias_list_buffer.size() + 1);
    for (auto& alias : __getserv_alias_list_buffer)
        __getserv_alias_list.unchecked_append(reinterpret_cast<char*>(alias.data()));
    __getserv_alias_list.unchecked_append(nullptr);

    __getserv_buffer.s_aliases = __getserv_alias_list.data();
    service_entry = &__getserv_buffer;

    if (!keep_service_file_open) {
        endservent();
    }
    return service_entry;
}

struct servent* getservbyname(const char* name, const char* protocol)
{
    if (name == nullptr)
        return nullptr;

    bool previous_file_open_setting = keep_service_file_open;
    setservent(1);
    struct servent* current_service = nullptr;
    auto service_file_handler = ScopeGuard([previous_file_open_setting] {
        if (!previous_file_open_setting) {
            endservent();
        }
    });

    while (true) {
        current_service = getservent();
        if (current_service == nullptr)
            break;
        else if (!protocol && strcmp(current_service->s_name, name) == 0)
            break;
        else if (strcmp(current_service->s_name, name) == 0 && strcmp(current_service->s_proto, protocol) == 0)
            break;
    }

    return current_service;
}
