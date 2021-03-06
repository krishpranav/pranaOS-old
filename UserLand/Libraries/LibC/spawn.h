/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sched.h>
#include <signal.h>
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

enum {
    POSIX_SPAWN_RESETIDS = 1 << 0,
    POSIX_SPAWN_SETPGROUP = 1 << 1,

    POSIX_SPAWN_SETSCHEDPARAM = 1 << 2,
    POSIX_SPAWN_SETSCHEDULER = 1 << 3,

    POSIX_SPAWN_SETSIGDEF = 1 << 4,
    POSIX_SPAWN_SETSIGMASK = 1 << 5,

    POSIX_SPAWN_SETSID = 1 << 6,
};

#define POSIX_SPAWN_SETSID POSIX_SPAWN_SETSID

struct posix_spawn_file_actions_state;
typedef struct {
    struct posix_spawn_file_actions_state* state;
} posix_spawn_file_actions_t;

typedef struct {
    short flags;
    pid_t pgroup;
    struct sched_param schedparam;
    int schedpolicy;
    sigset_t sigdefault;
    sigset_t sigmask;
} posix_spawnattr_t;

int posix_spawn(pid_t*, const char*, const posix_spawn_file_actions_t*, const posix_spawnattr_t*, char* const argv[], char* const envp[]);
int posix_spawnp(pid_t*, const char*, const posix_spawn_file_actions_t*, const posix_spawnattr_t*, char* const argv[], char* const envp[]);

int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t*, const char*);
int posix_spawn_file_actions_addfchdir(posix_spawn_file_actions_t*, int);
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t*, int);
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t*, int old_fd, int new_fd);
int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t*, int fd, const char*, int flags, mode_t);
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t*);
int posix_spawn_file_actions_init(posix_spawn_file_actions_t*);

int posix_spawnattr_destroy(posix_spawnattr_t*);
int posix_spawnattr_getflags(const posix_spawnattr_t*, short*);
int posix_spawnattr_getpgroup(const posix_spawnattr_t*, pid_t*);
int posix_spawnattr_getschedparam(const posix_spawnattr_t*, struct sched_param*);
int posix_spawnattr_getschedpolicy(const posix_spawnattr_t*, int*);
int posix_spawnattr_getsigdefault(const posix_spawnattr_t*, sigset_t*);
int posix_spawnattr_getsigmask(const posix_spawnattr_t*, sigset_t*);
int posix_spawnattr_init(posix_spawnattr_t*);
int posix_spawnattr_setflags(posix_spawnattr_t*, short);
int posix_spawnattr_setpgroup(posix_spawnattr_t*, pid_t);
int posix_spawnattr_setschedparam(posix_spawnattr_t*, const struct sched_param*);
int posix_spawnattr_setschedpolicy(posix_spawnattr_t*, int);
int posix_spawnattr_setsigdefault(posix_spawnattr_t*, const sigset_t*);
int posix_spawnattr_setsigmask(posix_spawnattr_t*, const sigset_t*);

__END_DECLS
