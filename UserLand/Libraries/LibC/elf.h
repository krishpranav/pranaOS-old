/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once


#ifndef KERNEL
#    include <sys/types.h>
#else
#    include <AK/Types.h>
#endif

typedef uint8_t Elf_Byte;

typedef uint32_t Elf32_Addr;
typedef uint32_t Elf32_Off; 
typedef int32_t Elf32_Sword;
typedef uint32_t Elf32_Word;
typedef uint16_t Elf32_Half;
typedef uint64_t Elf32_Lword;

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef int32_t Elf64_Shalf;

#ifdef __alpha__
typedef int64_t Elf64_Sword;
typedef uint64_t Elf64_Word;
#else
typedef int32_t Elf64_Sword;
typedef uint32_t Elf64_Word;
#endif

typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Lword;

typedef uint32_t Elf64_Half;
typedef uint16_t Elf64_Quarter;