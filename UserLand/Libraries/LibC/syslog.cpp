/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#define SYSLOG_NAMES 1

#include <AK/String.h>
#include <AK/StringBuilder.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>


extern "C" {

static struct syslog_data global_log_data = {
    .ident = nullptr,
    .logopt = 0,
    .facility = LOG_USER,
    .maskpri = LOG_UPTO(LOG_DEBUG)
};


static char program_name_buffer[256];
static bool program_name_set = false;


}