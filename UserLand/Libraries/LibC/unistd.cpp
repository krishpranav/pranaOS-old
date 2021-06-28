/*
 * Copyright (c) 2021 krishpranav, nuke123-sudo
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include <AK/ScopedValueRollback.h>
#include <AK/String.h>
#include <AK/Vector.h>
#include <alloca.h>
#include <assert.h>
#include <bits/pthread_integration.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <grp.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <syscall.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>