/*
 * Copyright (c) 2021 krishpranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct tm {
    int tm_sec;   
    int tm_min;   
    int tm_hour;  
    int tm_mday;  
    int tm_mon;   
    int tm_year;  
    int tm_wday;  
    int tm_yday;  
    int tm_isdst; 
};

__END_DECLS