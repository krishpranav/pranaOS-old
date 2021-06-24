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

__END_DECLS