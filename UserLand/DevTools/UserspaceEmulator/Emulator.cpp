/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

// includes
#include "Emulator.h"
#include "MmapRegion.h"
#include "SimpleRegion.h"
#include "SoftCPU.h"
#include <AK/Debug.h>
#include <AK/Format.h>
#include <AK/LexicalPath.h>
#include <AK/MappedFile.h>
#include <LibELF/AuxiliaryVector.h>
#include <LibELF/Image.h>
#include <LibELF/Validation.h>
#include <LibX86/ELFSymbolProvider.h>
#include <fcntl.h>
#include <syscall.h>
#include <unistd.h>

#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC optimize("O3")
#endif