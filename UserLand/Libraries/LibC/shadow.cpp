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

    if (min_string.is_empty())
        min_string = "-1";
    auto min_value = min_string.to_int();
    if (!min_value.has_value()) {
        dbgln("getspent(): Malformed min value on line {}", s_line_number);
        return false;
    }

    if (max_string.is_empty())
        max_string = "-1";
    auto max_value = max_string.to_int();
    if (!max_value.has_value()) {
        dbgln("getspent(): Malformed max value on line {}", s_line_number);
        return false;
    }

    if (warn_string.is_empty())
        warn_string = "-1";
    auto warn = warn_string.to_int();
    if (!warn.has_value()) {
        dbgln("getspent(): Malformed warn on line {}", s_line_number);
        return false;
    }

    if (inact_string.is_empty())
        inact_string = "-1";
    auto inact = inact_string.to_int();
    if (!inact.has_value()) {
        dbgln("getspent(): Malformed inact on line {}", s_line_number);
        return false;
    }

    if (expire_string.is_empty())
        expire_string = "-1";
    auto expire = expire_string.to_int();
    if (!expire.has_value()) {
        dbgln("getspent(): Malformed expire on line {}", s_line_number);
        return false;
    }

    if (flag_string.is_empty())
        flag_string = "0";
    auto flag = flag_string.to_int();
    if (!flag.has_value()) {
        dbgln("getspent(): Malformed flag on line {}", s_line_number);
        return false;
    }

    s_shadow_entry.sp_namp = const_cast<char*>(s_name.characters());
    s_shadow_entry.sp_pwdp = const_cast<char*>(s_pwdp.characters());
    s_shadow_entry.sp_lstchg = lstchg.value();
    s_shadow_entry.sp_min = min_value.value();
    s_shadow_entry.sp_max = max_value.value();
    s_shadow_entry.sp_warn = warn.value();
    s_shadow_entry.sp_inact = inact.value();
    s_shadow_entry.sp_expire = expire.value();
    s_shadow_entry.sp_flag = flag.value();

    return true;
}

struct spwd* getspent()
{
    if (!s_stream) 
        setspent();
    
    while (true) {
        if (!s_stream || feof(s_stream))
            return nullptr;
        
    }
}

}