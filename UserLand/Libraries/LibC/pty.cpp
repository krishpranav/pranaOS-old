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
    int rc = ptsname_r(*amaster, tty_name, sizeof(ttyname));
    if (rc < 0) {
        int error = errno;
        close(*amaster);
        errno = error;
        return -1;
    }
}