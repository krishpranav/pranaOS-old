/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/Format.h>
#include <errno.h>
#include <fcntl.h>
#include <pty.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int openpty(int* amaster, int* aslave, char* name, const struct termios* termp, const struct winsize* winp)
{
    *amaster = posix_openpt(O_RDWR);
    if (*amaster < 0) {
        return -1;
    }
    if (grantpt(*amaster) < 0) {
        int error = errno;
        close(*amaster);
        errno = error;
        return -1;
    }
    if (unlockpt(*amaster) < 0) {
        int error = errno;
        close(*amaster);
        errno = error;
        return -1;
    }

    char tty_name[32];
    int rc = ptsname_r(*amaster, tty_name, sizeof(tty_name));
    if (rc < 0) {
        int error = errno;
        close(*amaster);
        errno = error;
        return -1;
    }

    if (name) {
        [[maybe_unused]] auto rc = strlcpy(name, tty_name, 128);
    }

    *aslave = open(tty_name, O_RDWR | O_NOCTTY);
    if (*aslave < 0) {
        int error = errno;
        close(*amaster);
        errno = error;
        return -1;
    }
    if (termp) {
        if (tcsetattr(*aslave, TCSAFLUSH, termp) == -1) {
            int error = errno;
            close(*aslave);
            close(*amaster);
            errno = error;
            return -1;
        }
    }
    if (winp) {
        if (ioctl(*aslave, TIOCGWINSZ, winp) == -1) {
            int error = errno;
            close(*aslave);
            close(*amaster);
            errno = error;
            return -1;
        }
    }

    dbgln("openpty, master={}, slave={}, tty_name={}", *amaster, *aslave, tty_name);

    return 0;
}

pid_t forkpty(int* amaster, char* name, const struct termios* termp, const struct winsize* winp)
{
    int master;
    int slave;
    if (openpty(&master, &slave, name, termp, winp) < 0)
        return -1;
    pid_t pid = fork();
    if (pid < 0) {
        close(master);
        close(slave);
        return -1;
    }
    *amaster = master;
    if (pid == 0) {
        close(master);
        if (login_tty(slave) < 0)
            _exit(1);
        return 0;
    }
    close(slave);
    return pid;
}