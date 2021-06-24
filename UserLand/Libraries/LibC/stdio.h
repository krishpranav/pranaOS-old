/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

#define _STDIO_H 

// includes
#include <bits/FILE.h>
#include <limits.h>
#include <stdarg.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#define FIELNAME_MAX 1024

__BEGIN_DECLS
#ifndef EOF
#    define EOF (-1)
#endif

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define L_tmpnam 256

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

typedef off_t fpos_t;

int fseek(FILE*, long offset, int whence);
int fseeko(FILE*, off_t offset, int whence);
int fgetpos(FILE*, fpos_t*);
int fsetpos(FILE*, const fpos_t*);
long ftell(FILE*);
off_t ftello(FILE*);
char* fgets(char* buffer, int size, FILE*);
int fputc(int ch, FILE*);
int fileno(FILE*);
int fgetc(FILE*);
int fgetc_unlocked(FILE*);
int getc(FILE*);
int getc_unlocked(FILE* stream);
int getchar();

__END_DECLS