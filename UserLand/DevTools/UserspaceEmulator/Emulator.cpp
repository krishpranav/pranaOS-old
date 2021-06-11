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

namespace UserspaceEmulator {

static constexpr u32 stack_location = 0x10000000;
static constexpr size_t stack_size = 1 * MiB;

static Emulator* s_the;

Emulator& Emulator::the()
{
    VERIFY(s_the);
    return *s_the;
}

Emulator::Emulator(const String& executable_path, const Vector<String>& arguments, const Vector<String>& environment)
{
    
}

}