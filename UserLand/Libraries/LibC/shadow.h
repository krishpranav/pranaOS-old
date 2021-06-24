/*
 * Copyright (c) 2021 nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

#ifndef AK_OS_MACOS
#    include <bits/FILE.h>
#endif
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct spwd {
    char* sp_namp;
    char* sp_pwdp;
    long int sp_lstchg;
    long int sp_min;
    long int sp_max;
    long int sp_warn;
    long int sp_inact;
    long int sp_expire;
    unsigned long int sp_flag;
};

#ifndef AK_OS_MACOS
struct spwd* getspent();


__END_DECLS
