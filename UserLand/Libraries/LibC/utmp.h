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

__END_DECLS