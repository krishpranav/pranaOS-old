/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */


#pragma once

// includes
#include <LibGUI/Model.h>
#include <LibX86/Instruction.h>

namespace Profiler {

class Profile;
class ProfileNode;

struct InstructionData {
    X86::Instruction insn;
    String disassembly;
    StringView bytes;
    FlatPtr address { 0 };
    u32 event_count { 0 };
    float percent { 0 };
};


}