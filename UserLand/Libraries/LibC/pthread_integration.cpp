/*
 * Copyright (c) 2021, Krisna Pranav, evilbat831, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Atomic.h>
#include <AK/NeverDestroyed.h>
#include <AK/Vector.h>
#include <bits/pthread_integration.h>
#include <errno.h>
#include <sched.h>
#include <unistd.h>

namespace {

static Atomic<bool> g_did_touch_atfork { false };
static pthread_mutex_t g_atfork_list_mutex __PTHREAD_MUTEX_INITIALIZER;
static NeverDestroyed<Vector<void (*)(void), 4>> g_atfork_prepare_list;
static NeverDestroyed<Vector<void (*)(void), 4>> g_atfork_child_list;
static NeverDestroyed<Vector<void (*)(void), 4>> g_atfork_parent_list;


}