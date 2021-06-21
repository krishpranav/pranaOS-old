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

#define EI_MAG0 0       /* file ID */
#define EI_MAG1 1       /* file ID */
#define EI_MAG2 2       /* file ID */
#define EI_MAG3 3       /* file ID */
#define EI_CLASS 4      /* file class */
#define EI_DATA 5       /* data encoding */
#define EI_VERSION 6    /* ELF header version */
#define EI_OSABI 7      /* OS/ABI ID */
#define EI_ABIVERSION 8 /* ABI version */
#define EI_PAD 9        /* start of pad bytes */
#define EI_NIDENT 16    /* Gfx::Size of e_ident[] */

#define ELFMAG0 0x7f     /* e_ident[EI_MAG0] */
#define ELFMAG1 'E'      /* e_ident[EI_MAG1] */
#define ELFMAG2 'L'      /* e_ident[EI_MAG2] */
#define ELFMAG3 'F'      /* e_ident[EI_MAG3] */
#define ELFMAG "\177ELF" /* magic */
#define SELFMAG 4        /* size of magic */