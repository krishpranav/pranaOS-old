/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Types.h>
#include <fenv.h>

static_assert(sizeof(__x87_floating_point_environment) == 28);
