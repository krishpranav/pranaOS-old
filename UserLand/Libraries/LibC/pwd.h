/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <bits/FILE.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

struct passwd {
    char* pw_name;
    char* pw_passwd;
    uid_t pw_uid;
    gid_t pw_gid;
    char* pw_gecos;
    char* pw_dir;
    char* pw_shell;
};

struct passwd* getpwent();
void setpwent();
void endpwent();
struct passwd* getpwnam(const char* name);
struct passwd* getpwuid(uid_t);
int putpwent(const struct passwd* p, FILE* stream);

int getpwnam_r(const char* name, struct passwd* pwd, char* buf, size_t buflen, struct passwd** result);
int getpwuid_r(uid_t, struct passwd* pwd, char* buf, size_t buflen, struct passwd** result);

__END_DECLS
