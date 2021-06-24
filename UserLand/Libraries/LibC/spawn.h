/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <spawn.h>
#include <AK/Function.h>
#include <AK/Vector.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct posix_spawn_file_actions_state {
    Vector<Function<int()>, 4> actions;
};

[[noreturn]] static void posix_spawn_child(const char* path, const posix_spawn_file_actions_t* file_actions, const posix_spawnattr_t* attr, char* const argv[], char* const envp[], int (*exec)(const char*, char* const[], char* const[]))
{
    
}