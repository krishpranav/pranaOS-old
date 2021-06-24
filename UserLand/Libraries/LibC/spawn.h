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