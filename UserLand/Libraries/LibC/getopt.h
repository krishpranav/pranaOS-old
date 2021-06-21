/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct option {
    const char* name;
    int has_arg;
    int* flag;
    int val;
};

extern int opterr;
extern int optopt
extern int optind;
extern int optreset;
extern char* optarg;