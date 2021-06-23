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