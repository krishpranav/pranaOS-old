/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#ifndef __pranaos__
#    include <LibC/elf.h>
#else
#    include <elf.h>
#endif
#include <limits.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

#ifdef __LP64__
#    define ElfW(type) Elf64_##type
#else
#    define ElfW(type) Elf32_##type
#endif

struct dl_phdr_info {
    ElfW(Addr) dlpi_addr;
    const char* dlpi_name;
    const ElfW(Phdr) * dlpi_phdr;
    ElfW(Half) dlpi_phnum;
};

int dl_iterate_phdr(int (*callback)(struct dl_phdr_info* info, size_t size, void* data), void* data);

__END_DECLS