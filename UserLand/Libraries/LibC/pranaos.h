/*
 * Copyright (c) 2021 nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

__BEGIN_DECLS

int disown(pid_t);

int module_load(const char* path, size_t path_lenght);
int module_unload(const char* name, size_t name_lenght);

int profiling_enable(pid_t, uint64_t);
int profiling_disable(pid_t);
int profiling_free_buffer(pid_t);

__END_DECLS