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

ALWAYS_INLINE void SoftCPU::warn_if_flags_tainted(const char* message) const
{
    if (m_flags_tainted) [[unlikely]] {
        reportln("\n=={}==  \033[31;1mConditional depends on uninitialized data\033[0m ({})\n", getpid(), message);
        Emulator::the().dump_backtrace();
    }
}

template<typename T, typename U>
constexpr T sign_extended_to(U value)
{
    if (!(value & X86::TypeTrivia<U>::sign_bit))
        return value;
    return (X86::TypeTrivia<T>::mask & ~X86::TypeTrivia<U>::mask) | value;
}

SoftCPU::SoftCPU(Emulator& emulator)
    : m_emulator(emulator)
{
    memset(m_gpr, 0, sizeof(m_gpr));
    memset(m_gpr_shadow, 1, sizeof(m_gpr_shadow));

    m_segment[(int)X86::SegmentRegister::CS] = 0x1b;
    m_segment[(int)X86::SegmentRegister::DS] = 0x23;
    m_segment[(int)X86::SegmentRegister::ES] = 0x23;
    m_segment[(int)X86::SegmentRegister::SS] = 0x23;
    m_segment[(int)X86::SegmentRegister::GS] = 0x2b;
}

void SoftCPU::dump() const
{
    outln(" eax={:08x}  ebx={:08x}  ecx={:08x}  edx={:08x}  ebp={:08x}  esp={:08x}  esi={:08x}  edi={:08x} o={:d} s={:d} z={:d} a={:d} p={:d} c={:d}",
        eax(), ebx(), ecx(), edx(), ebp(), esp(), esi(), edi(), of(), sf(), zf(), af(), pf(), cf());
    outln("#eax={:08x} #ebx={:08x} #ecx={:08x} #edx={:08x} #ebp={:08x} #esp={:08x} #esi={:08x} #edi={:08x} #f={}",
        eax().shadow(), ebx().shadow(), ecx().shadow(), edx().shadow(), ebp().shadow(), esp().shadow(), esi().shadow(), edi().shadow(), m_flags_tainted);
    fflush(stdout);
}

void SoftCPU::update_code_cache()
{
    auto* region = m_emulator.mmu().find_region({ cs(), eip() });
    VERIFY(region);

    if (!region->is_executable()) {
        reportln("SoftCPU::update_code_cache: Non-executable region @ {:p}", eip());
        Emulator::the().dump_backtrace();
        TODO();
    }

    m_cached_code_region = region;
    m_cached_code_base_ptr = region->data();
}

ValueWithShadow<u8> SoftCPU::read_memory8(X86::LogicalAddress address)
{
    VERIFY(address.selector() == 0x1b || address.selector() == 0x23 || address.selector() == 0x2b);
    auto value = m_emulator.mmu().read8(address);
    outln_if(MEMORY_DEBUG, "\033[36;1mread_memory8: @{:04x}:{:08x} -> {:02x} ({:02x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    return value;
}



ValueWithShadow<u16> SoftCPU::read_memory16(X86::LogicalAddress address)
{
    VERIFY(address.selector() == 0x1b || address.selector() == 0x23 || address.selector() == 0x2b);
    auto value = m_emulator.mmu().read16(address);
    outln_if(MEMORY_DEBUG, "\033[36;1mread_memory16: @{:04x}:{:08x} -> {:04x} ({:04x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    return value;   
}

ValueWithShadow<u32> SoftCPU::read_memory32(X86::LogicalAddress address)
{
    VERIFY(address.selector() == 0x1b || address.selector() == 0x23 || address.selector() == 0x2b);
    auto value = m_emulator.mmu().read32(address);
    outln_if(MEMORY_DEBUG, "\033[36;1mread_memory32: @{:04x}:{:08x} -> {:08x} ({:08x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    return value;
}

ValueWithShadow<u64> SoftCPU::read_memory64(X86::LogicalAddress address)
{
    VERIFY(address.selector() == 0x1b || address.selector() == 0x23 || address.selector() == 0x2b);
    auto value = m_emulator.mmu().read64(address);
    outln_if(MEMORY_DEBUG, "\033[36;1mread_memory64: @{:04x}:{:08x} -> {:016x} ({:016x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    return value;
}

ValueWithShadow<u128> SoftCPU::read_memory128(X86::LogicalAddress address)
{
    VERIFY(address.selector() == 0x1b || address.selector() == 0x23 || address.selector() == 0x2b);
    auto value = m_emulator.mmu().read128(address);
    outln_if(MEMORY_DEBUG, "\033[36;1mread_memory128: @{:04x}:{:08x} -> {:032x} ({:032x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    return value;
}

ValueWithShadow<u256> SoftCPU::read_memory256(X86::LogicalAddress address)
{
    VERIFY(address.selector() == 0x1b || address.selector() == 0x23 || address.selector() == 0x2b);
    auto value = m_emulator.mmu().read256(address);
    outln_if(MEMORY_DEBUG, "\033[36;1mread_memory256: @{:04x}:{:08x} -> {:064x} ({:064x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    return value;
}

void SoftCPU::write_memory8(X86::LogicalAddress address, ValueWithShadow<u8> value)
{
    VERIFY(address.selector() == 0x23 || address.selector() == 0x2b);
    outln_if(MEMORY_DEBUG, "\033[36;1mwrite_memory8: @{:04x}:{:08x} <- {:02x} ({:02x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    m_emulator.mmu().write8(address, value);
}

void SoftCPU::write_memory16(X86::LogicalAddress address, ValueWithShadow<u16> value)
{
    VERIFY(address.selector() == 0x23 || address.selector() == 0x2b);
    outln_if(MEMORY_DEBUG, "\033[36;1mwrite_memory16: @{:04x}:{:08x} <- {:04x} ({:04x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    m_emulator.mmu().write16(address, value);
}

void SoftCPU::write_memory32(X86::LogicalAddress address, ValueWithShadow<u32> value)
{
    VERIFY(address.selector() == 0x23 || address.selector() == 0x2b);
    outln_if(MEMORY_DEBUG, "\033[36;1mwrite_memory32: @{:04x}:{:08x} <- {:08x} ({:08x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    m_emulator.mmu().write32(address, value);
}


}