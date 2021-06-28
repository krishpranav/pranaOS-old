/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <syscall.h>

extern "C" {

void* pranaos_mmap(void* addr, size_t size, int prot, int flags, int fd, off_t offset, size_t alignment, const char* name)
{
    Syscall::SC_mmap_params params { (uintptr_t)addr, size, alignment, prot, flags, fd, offset, { name, name ? strlen(name) : 0 } };
    ptrdiff_t rc = syscall(SC_mmap, &params);
    if (rc < 0 && -rc < EMAXERRNO) {
        errno = -rc;
        return MAP_FAILED;
    }
    return (void*)rc;
}

void* mmap(void* addr, size_t size, int prot, int flags, int fd, off_t offset)
{
    return pranaos_mmap(addr, size, prot, flags, fd, offset, PAGE_SIZE, nullptr);
}

void* mmap_with_name(void* addr, size_t size, int prot, int flags, int fd, off_t offset, const char* name)
{
    return pranaos_mmap(addr, size, prot, flags, fd, offset, PAGE_SIZE, name);
}

}