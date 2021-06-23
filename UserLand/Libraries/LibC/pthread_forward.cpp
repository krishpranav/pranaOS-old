/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Assertions.h>
#include <LibC/bits/pthread_forward.h>

static PthreadFunctions s_pthread_functions;

void __init_pthread_forward(PthreadFunctions funcs)
{
  s_pthread_functions = funcs;
}
