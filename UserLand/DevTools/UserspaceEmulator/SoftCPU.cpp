/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "SoftCPU.h"
#include "Emulator.h"
#include <AK/Assertions.h>
#include <AK/BitCast.h>
#include <AK/Debug.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if defined(__GNUC__) && !defined(__clang__)
#     pragma GCC optimize("O3")
#endif
