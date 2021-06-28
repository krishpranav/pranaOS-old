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

}