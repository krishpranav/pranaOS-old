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

}

}