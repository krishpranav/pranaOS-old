/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <bits/stdint.h>

#ifndef PAGE_SIZE
#    define PAGE_SIZE 4096
#endif

#define HOST_NAME_MAX 64

#define PATH_MAX 4096
#if !defined MAXPATHLEN && defined PATH_MAX
#    define MAXPATHLEN PATH_MAX
#endif