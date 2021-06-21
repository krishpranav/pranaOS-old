/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/StringView.h>
#include <AK/Vector.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// opt values 
int opterr = 1;
int optopt = 0;
int optind = 1;
int optreset = 0;
char* optarg = nullptr;

static size_t s_index_into_multioption_argument = 0;

static inline void report_error(const char* format, ...)
{
    if (!opterr)
        return;

    fputs("\033[31m", stderr);

    va_list ap;
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    fputs("\033[0m\n", stderr);
}