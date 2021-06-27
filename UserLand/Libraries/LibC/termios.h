/*
 * Copyright (c) 2021 krishpranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include <sys/cdefs.h>
#include <sys/types.h>

__BEGIN_DECLS

#define NCCS 32

typedef uint32_t tcflag_t;
typedef uint8_t cc_t;
typedef uint32_t speed_t;

struct termios {
    tcflag_t c_iflag;
    tcflag_t c_oflag;
    tcflag_t c_cflag;
    tcflag_t c_lflag;
    cc_t c_cc[NCCS];
    speed_t c_ispeed;
    speed_t c_ospeed;
};

int tcgetattr(int fd, struct termios*);
int tcsetattr(int fd, int optional_actions, const struct termios*);
int tcflow(int fd, int action);
int tcflush(int fd, int queue_selector);


speed_t cfgetispeed(const struct termios*);
speed_t cfgetospeed(const struct termios*);
int cfsetispeed(struct termios*, speed_t);
int cfsetospeed(struct termios*, speed_t);
void cfmakeraw(struct termios*);

__END_DECLS