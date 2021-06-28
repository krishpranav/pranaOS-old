/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/time.h>

__BEGIN_DECLS


struct exit_status {         
    short int e_termination; 
    short int e_exit;        
};

#define USER_PROCESS 7
#define DEAD_PROCESS 8

#define UT_NAMESIZE 32
#define UT_LINESIZE 32
#define UT_HOSTSIZE 256

__END_DECLS