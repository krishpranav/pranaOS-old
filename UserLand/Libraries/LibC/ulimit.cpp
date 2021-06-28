/*
 * Copyright (c) 2021 krishpranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Format.h>
#include <assert.h>
#include <sys/resource.h>
#include <ulimit.h>

extern "C" {


long ulimit([[maybe_unused]] int cmd, [[maybe_unused]] long newlimit)
{
    dbgln("FIXME: Implement getrusage()");
    TODO();
    return -1;
};

}