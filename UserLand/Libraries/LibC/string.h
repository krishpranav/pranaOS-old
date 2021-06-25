/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once 

// includes
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

size_t strlen(const char*);
size_t strnlen(const char*, size_t maxlen);

int strcmp(const char*, const char*);
int strncmp(const char*, const char*, size_t);

int memcmp(const void*, const void*, size_t);
void* memcpy(void*, const void*, size_t);
void* memmove(void*, const void*, size_t);
void* memchr(const void*, int c, size_t);
const void* memmem(const void* haystack, size_t, const void* needle, size_t);

void* memset(void*, int, size_t);
void explicit_bzero(void*, size_t) __attribute__((nonnull(1)));


__END_DECLS