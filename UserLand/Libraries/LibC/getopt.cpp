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