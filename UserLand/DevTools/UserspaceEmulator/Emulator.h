/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

// incldes
#include "MallocTracer.h"
#include "RangeAllocator.h"
#include "Report.h"
#include "SoftCPU.h"
#include "SoftMMU.h"
#include <AK/MappedFile.h>
#include <AK/Types.h>
#include <LibDebug/DebugInfo.h>
#include <LibELF/AuxiliaryVector.h>
#include <LibELF/Image.h>
#include <LibX86/Instruction.h>
#include <signal.h>
#include <sys/types.h>

namespace  UserspaceEmulator {

class MallocTracer;

class Emulator {
public:
    static Emulator& the();

    Emulator(const String& executable_path, const Vector<String>& arguments, const Vector<String>& environment);

    bool load_elf();
    void dump_backtrace();
    void dump_backtrace(const Vector<FlatPtr>&);
    Vector<FlatPtr> raw_backtrace();

    int exec();
    u32 virt_syscall(u32 function, u32 arg1, u32 arg2, u32 arg3);

    SoftMMU& mmu() { return m_mmu; }

    MallocTracer* malloc_tracer() { return m_malloc_tracer; }

    bool is_in_malloc_or_free() const;
    bool is_int_loader_code() const;
    bool is_in_libsystem() const;
    bool is_in_libc() const;

    void did_receive_signal(int signum) { m_pending_signals |= (1 << signum); }

    void dump_regions() const;

}

}