/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Debug.h>
#include <AK/Format.h>
#include <LibC/bits/pthread_integration.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/internals.h>
#include <sys/mman.h>
#include <unistd.h>

extern "C" {

struct AtExitEntry {
    AtExitFunction method { nullptr };
    void* parameter { nullptr };
    void* dso_handle { nullptr };
    bool has_been_called { false };
};

}