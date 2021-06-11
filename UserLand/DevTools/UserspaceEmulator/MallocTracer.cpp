/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "MallocTracer.h"
#include "Emulator.h"
#include "MmapRegion.h"
#include <AK/Debug.h>
#include <AK/TemporaryChange.h>
#include <mallocdefs.h>
#include <string.h>
#include <unistd.h>