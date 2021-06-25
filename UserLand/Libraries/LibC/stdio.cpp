/*
 * Copyright (c) 2021 krishpranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Format.h>
#include <AK/PrintfImplementation.h>
#include <AK/ScopedValueRollback.h>
#include <AK/StdLibExtras.h>
#include <AK/String.h>
#include <LibC/bits/pthread_integration.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>

struct FILE {
public:
    FILE(int fd, int mode)
        : m_fd(fd)
        , m_mode(mode)
    {
        __pthread_mutex_init(&m_mutex, nullptr)
    }
    ~FILE();

}