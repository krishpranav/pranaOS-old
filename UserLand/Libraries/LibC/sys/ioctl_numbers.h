/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <sys/cdefs.h>

__BEGIN_DECLS

struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

struct FBResolution {
    unsigned pitch;
    unsigned width;
    unsigned height;
};

struct FBRect {
    unsigned x;
    unsigned y;
    unsigned width;
    unsigned height;
};

struct FBRects {
    unsigned count;
    FBRect const* rects;
};

__END_DECLS

enum IOCtlNumber {
    TIOCGPGRP,
    TIOCSPGRP,
    TCGETS,
    TCSETS,
    TCSETSW,
    TCSETSF,
    TCFLSH,
    TIOCGWINSZ,
    TIOCSCTTY,
    TIOCSTI,
    TIOCNOTTY,
    TIOCSWINSZ,
    FB_IOCTL_GET_SIZE_IN_BYTES,
    FB_IOCTL_GET_RESOLUTION,
    FB_IOCTL_SET_RESOLUTION,
    FB_IOCTL_GET_BUFFER,
    FB_IOCTL_SET_BUFFER,
    FB_IOCTL_FLUSH_BUFFERS,
    SIOCSIFADDR,
    SIOCGIFADDR,
    SIOCGIFHWADDR,
    SIOCGIFNETMASK,
    SIOCSIFNETMASK,
    SIOCGIFBRDADDR,
    SIOCGIFMTU,
    SIOCGIFFLAGS,
    SIOCGIFCONF,
    SIOCADDRT,
    SIOCDELRT,
    FIBMAP,
    FIONBIO,
};