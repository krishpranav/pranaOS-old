/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/String.h>
#include <AK/TemporaryChange.h>
#include <AK/Vector.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C" {

static FILE* s_stream = nullptr;
static unsigned s_line_number = 0;
static struct passwd s_passwd_entry;

static String s_name;
static String s_passwd;
static String s_gecos;
static String s_dir;
static String s_shell;

void setpwent()
{
    s_line_number = 0;
    if (s_stream) {
        rewind(s_stream);
    } else {
        s_stream = fopen("/etc/passwd", "r");
        if (!s_stream) {
            perror("open /etc/passwd");
        }
    }
}

}