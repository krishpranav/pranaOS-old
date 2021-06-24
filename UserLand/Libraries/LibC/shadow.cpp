/*
 * Copyright (c) 2021 nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/String.h>
#include <AK/TemporaryChange.h>
#include <AK/Vector.h>
#include <errno.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern "C" {

static FILE* s_stream = nullptr;
static unsigned s_line_number = 0;
static struct spwd s_shadow_entry;

static String s_name;
static String s_pwdp;

void setspent()
{
    s_line_number = 0;
    if (s_stream) {
        rewind(s_stream);
    } else {
        s_stream = fopen("/etc/shadow", "r");
        if (!s_stream) {
            dbgln("open /etc/shadow failed: {}", strerror(errno));
        }
    }
}


void endspent()
{
    s_line_number = 0;
    if (s_stream) {
        fclose(s_stream);
        s_stream = nullptr;
    }

    memset(&s_shadow_entry, 0, sizeof(s_shadow_entry));

    s_name = {};
    s_pwdp = {};
}

struct spwd* getspnam(const char* name)
{
    setspent();
    while (auto* sp = getspent()) {
        if (!strcmp(sp->sp_namp, name)) {
            return sp;
        }
    }
    return nullptr;
}

static bool parse_shadow_entry(const String& line)
{
    auto parts = line.split_view(':', true);
    if (parts.size() != 9) {
        dbgln("getspent(): Malformed entry on line {}", s_line_number);
        return false;
    }

    s_name = parts[0];
    s_pwdp = parts[1];
    auto& lstchg_string = parts[2];
    auto& min_string = parts[3];
    auto& max_string = parts[4];
    auto& warn_string = parts[5];
    auto& inact_string = parts[6];
    auto& expire_string = parts[7];
    auto& flag_string = parts[8];

    auto lstchg = lstchg_string.to_int();
    if (!lstchg.has_value()) {
        dbgln("getspent(): Malformed lstchg on line {}", s_line_number);
        return false;
    }

}

}