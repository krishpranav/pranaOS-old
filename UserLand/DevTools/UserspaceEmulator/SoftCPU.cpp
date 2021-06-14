/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

// includes
#include "SoftCPU.h"
#include "Emulator.h"
#include <AK/Assertions.h>
#include <AK/BitCast.h>
#include <AK/Debug.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if defined(__GNUC__) && !defined(__clang__)
#    pragma GCC optimize("O3")
#endif

#define TODO_INSN()                                                                   \
    do {                                                                              \
        reportln("\n=={}== Unimplemented instruction: {}\n", getpid(), __FUNCTION__); \
        m_emulator.dump_backtrace();                                                  \
        _exit(0);                                                                     \
    } while (0)

#define DEFINE_GENERIC_SHIFT_ROTATE_INSN_HANDLERS(mnemonic, op)                                                                            \
    void SoftCPU::mnemonic##_RM8_1(const X86::Instruction& insn) { generic_RM8_1(op<ValueWithShadow<u8>>, insn); }                         \
    void SoftCPU::mnemonic##_RM8_CL(const X86::Instruction& insn) { generic_RM8_CL(op<ValueWithShadow<u8>>, insn); }                       \
    void SoftCPU::mnemonic##_RM8_imm8(const X86::Instruction& insn) { generic_RM8_imm8<true, false>(op<ValueWithShadow<u8>>, insn); }      \
    void SoftCPU::mnemonic##_RM16_1(const X86::Instruction& insn) { generic_RM16_1(op<ValueWithShadow<u16>>, insn); }                      \
    void SoftCPU::mnemonic##_RM16_CL(const X86::Instruction& insn) { generic_RM16_CL(op<ValueWithShadow<u16>>, insn); }                    \
    void SoftCPU::mnemonic##_RM16_imm8(const X86::Instruction& insn) { generic_RM16_unsigned_imm8<true>(op<ValueWithShadow<u16>>, insn); } \
    void SoftCPU::mnemonic##_RM32_1(const X86::Instruction& insn) { generic_RM32_1(op<ValueWithShadow<u32>>, insn); }                      \
    void SoftCPU::mnemonic##_RM32_CL(const X86::Instruction& insn) { generic_RM32_CL(op<ValueWithShadow<u32>>, insn); }                    \
    void SoftCPU::mnemonic##_RM32_imm8(const X86::Instruction& insn) { generic_RM32_unsigned_imm8<true>(op<ValueWithShadow<u32>>, insn); }

namespace UserspaceEmulator {

template<typename T>
ALWAYS_INLINE void warn_if_uninitialized(T value_with_shadow, const char* message)
{
    if (value_with_shadow.is_uninitialized()) [[unlikely]] {
        reportln("\033[31;1mWarning! Use of uninitialized value: {}\033[0m\n", message);
        Emulator::the().dump_backtrace();
    }
}

}