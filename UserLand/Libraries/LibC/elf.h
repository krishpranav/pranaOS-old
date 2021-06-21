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

#define ELFCLASSNONE 0 /* invalid */
#define ELFCLASS32 1   /* 32-bit objs */
#define ELFCLASS64 2   /* 64-bit objs */
#define ELFCLASSNUM 3  /* number of classes */

#define ELFDATANONE 0 /* invalid */
#define ELFDATA2LSB 1 /* Little-Endian */
#define ELFDATA2MSB 2 /* Big-Endian */
#define ELFDATANUM 3  /* number of data encode defines */

#define ELFOSABI_SYSV 0         /* UNIX System V ABI */
#define ELFOSABI_HPUX 1         /* HP-UX operating system */
#define ELFOSABI_NETBSD 2       /* NetBSD */
#define ELFOSABI_LINUX 3        /* GNU/Linux */
#define ELFOSABI_HURD 4         /* GNU/Hurd */
#define ELFOSABI_86OPEN 5       /* 86Open common IA32 ABI */
#define ELFOSABI_SOLARIS 6      /* Solaris */
#define ELFOSABI_MONTEREY 7     /* Monterey */
#define ELFOSABI_IRIX 8         /* IRIX */
#define ELFOSABI_FREEBSD 9      /* FreeBSD */
#define ELFOSABI_TRU64 10       /* TRU64 UNIX */
#define ELFOSABI_MODESTO 11     /* Novell Modesto */
#define ELFOSABI_OPENBSD 12     /* OpenBSD */
#define ELFOSABI_ARM 97         /* ARM */
#define ELFOSABI_STANDALONE 255 /* Standalone (embedded) application */

#define IS_ELF(ehdr) ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && (ehdr).e_ident[EI_MAG1] == ELFMAG1 && (ehdr).e_ident[EI_MAG2] == ELFMAG2 && (ehdr).e_ident[EI_MAG3] == ELFMAG3)

typedef struct elfhdr {
    unsigned char e_ident[EI_NIDENT]; /* ELF Identification */
    Elf32_Half e_type;                /* object file type */
    Elf32_Half e_machine;             /* machine */
    Elf32_Word e_version;             /* object file version */
    Elf32_Addr e_entry;               /* virtual entry point */
    Elf32_Off e_phoff;                /* program header table offset */
    Elf32_Off e_shoff;                /* section header table offset */
    Elf32_Word e_flags;               /* processor-specific flags */
    Elf32_Half e_ehsize;              /* ELF header size */
    Elf32_Half e_phentsize;           /* program header entry size */
    Elf32_Half e_phnum;               /* number of program header entries */
    Elf32_Half e_shentsize;           /* section header entry size */
    Elf32_Half e_shnum;               /* number of section header entries */
    Elf32_Half e_shstrndx;            /* section header table's "section
					   header string table" entry offset */
} Elf32_Ehdr;
