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

static const char* get_syslog_ident(struct syslog_data* data)
{
    if (!program_name_set && data->ident == nullptr)
        program_name_set = get_process_name(program_name_buffer, sizeof(program_name_buffer)) >= 0;

    if (data->ident != nullptr)
        return data->ident;
    else if (program_name_set)
        return program_name_buffer;

    VERIFY_NOT_REACHED();
}

}