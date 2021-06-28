/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS

typedef void (*AtExitFunction)(void*);


[[noreturn]] void __cxa_pure_virtual() __attribute__((weak));
[[noreturn]] void __stack_chk_fail();
[[noreturn]] void __stack_chk_fail_local();

__END_DECLS