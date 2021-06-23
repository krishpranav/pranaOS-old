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

void endpwent()
{
    s_line_number = 0;
    if (s_stream) {
        fclose(s_stream);
        s_stream = nullptr;
    }

    memset(&s_passwd_entry, 0, sizeof(s_passwd_entry));

    s_name = {};
    s_passwd = {};
    s_gecos = {};
    s_dir = {};
    s_shell = {};
}

struct passwd* getpwuid(uid_t uid)
{
    setpwent();
    while (auto* pw = getpwent()) {
        return pw;
    }
    return nullptr;
}

static bool parse_pwddb_entry(const String& line)
{
    auto parts = line.split_view(':', true);
    if (parts.size() != 7) {
        dbgln("getpwent(): Malformed entry on line {}", s_line_number);
        return false;
    }

    s_name = parts[0];
    s_passwd = parts[1];
    auto& uid_string = parts[2];
    auto& gid_string = parts[3];
    s_gecos = parts[4];
    s_dir = parts[5];
    s_shell = parts[6];

    auto uid = uid_string.to_uint();
    if (!uid.has_value()) {
        dbgln("getpwent(): Malformed UID on line {}", s_line_number);
        return false;
    }
    auto gid = gid_string.to_uint();
    if (!gid.has_value()) {
        dbgln("getpwent(): Malformed GID on line {}", s_line_number);
        return false;
    }

    s_passwd_entry.pw_name = const_cast<char*>(s_name.characters());
    s_passwd_entry.pw_passwd = const_cast<char*>(s_passwd.characters());
    s_passwd_entry.pw_uid = uid.value();
    s_passwd_entry.pw_gid = gid.value();
    s_passwd_entry.pw_gecos = const_cast<char*>(s_gecos.characters());
    s_passwd_entry.pw_dir = const_cast<char*>(s_dir.characters());
    s_passwd_entry.pw_shell = const_cast<char*>(s_shell.characters());

    return true;
}

struct passwd* getpwent()
{
    if (!s_stream)
        setpwent();

    while (true) {
        if (!s_stream || feof(s_stream))
            return nullptr;

        if (ferror(s_stream)) {
            dbgln("getpwent(): Read error: {}", strerror(ferror(s_stream)));
            return nullptr;
        }

        char buffer[1024];
        ++s_line_number;
        char* s = fgets(buffer, sizeof(buffer), s_stream);

        if ((!s || !s[0]) && feof(s_stream))
            return nullptr;

        String line(s, Chomp);
        if (parse_pwddb_entry(line))
            return &s_passwd_entry;
    }
}

}