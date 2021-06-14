/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/


#pragma once

// includes
#include "Region.h"
#include "ValueWithShadow.h"
#include <AK/HashMap.h>
#include <AK/NonnullOwnPtrVector.h>
#include <AK/OwnPtr.h>
#include <AK/Types.h>
#include <LibX86/Instruction.h>

namespace UserspaceEmulator {

class Emulator;

class SoftMMU {
public:
    explicit SoftMMU(Emulator&);

    ValueWithShadow<u8> read8(X86::LogicalAddress);
    ValueWithShadow<u16> read16(X86::LogicalAddress);
    ValueWithShadow<u32> read32(X86::LogicalAddress);
    
}

}