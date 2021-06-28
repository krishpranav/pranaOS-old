/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <assert.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>

extern "C" {

int tcgetattr(int fd, struct termios* t)
{
    return ioctl(fd, TCGETS, t);
}

int tcsetattr(int fd, int optional_actions, const struct termios* t)
{
    switch (optional_actions) {
    case TCSANOW:
        return ioctl(fd, TCSETS, t);
    case TCSADRAIN:
        return ioctl(fd, TCSETSW, t);
    case TCSAFLUSH:
        return ioctl(fd, TCSETSF, t);
    }
    errno = EINVAL;
    return -1;
}

int tcflow([[maybe_unused]] int fd, [[maybe_unused]] int action)
{
    errno = EINVAL;
    return -1;
}

}