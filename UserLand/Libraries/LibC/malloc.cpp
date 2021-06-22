/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


// includes
#include <AK/Debug.h>
#include <AK/ScopedValueRollback.h>
#include <AK/Vector.h>
#include <LibELF/AuxiliaryVector.h>
#include <LibThreading/Lock.h>
#include <assert.h>
#include <errno.h>
#include <mallocdefs.h>
#include <serenity.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/internals.h>
#include <sys/mman.h>
#include <syscall.h>