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

void SoftCPU::write_memory64(X86::LogicalAddress address, ValueWithShadow<u64> value)
{
    VERIFY(address.selector() == 0x23 || address.selector() == 0x2b);
    outln_if(MEMORY_DEBUG, "\033[36;1mwrite_memory64: @{:04x}:{:08x} <- {:016x} ({:016x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    m_emulator.mmu().write64(address, value);
}

void SoftCPU::write_memory128(X86::LogicalAddress address, ValueWithShadow<u128> value)
{
    VERIFY(address.selector() == 0x23 || address.selector() == 0x2b);
    outln_if(MEMORY_DEBUG, "\033[36;1mwrite_memory128: @{:04x}:{:08x} <- {:032x} ({:032x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    m_emulator.mmu().write128(address, value);
}

void SoftCPU::write_memory256(X86::LogicalAddress address, ValueWithShadow<u256> value)
{
    VERIFY(address.selector() == 0x23 || address.selector() == 0x2b);
    outln_if(MEMORY_DEBUG, "\033[36;1mwrite_memory256: @{:04x}:{:08x} <- {:064x} ({:064x})\033[0m", address.selector(), address.offset(), value, value.shadow());
    m_emulator.mmu().write256(address, value);
}

void SoftCPU::push_string(const StringView& string)
{
    size_t space_to_allocate = round_up_to_power_of_two(string.length() + 1, 16);
    set_esp({ esp().value() - space_to_allocate, esp().shadow() });
    m_emulator.mmu().copy_to_vm(esp().value(), string.characters_without_null_termination(), string.length());
    m_emulator.mmu().write8({ 0x23, esp().value() + string.length() }, shadow_wrap_as_initialized((u8)'\0'));
}

void SoftCPU::push_buffer(const u8* data, size_t size)
{
    set_esp({ esp().value() - size, esp().shadow() });
    warn_if_uninitialized(esp(), "push_buffer");
    m_emulator.mmu().copy_to_vm(esp().value(), data, size);
}

void SoftCPU::push32(ValueWithShadow<u32> value)
{
    set_esp({ esp().value() - sizeof(u32), esp().shadow() });
    warn_if_uninitialized(esp(), "push32");
    write_memory32({ ss(), esp().value() }, value);
}

ValueWithShadow<u32> SoftCPU::pop32()
{
    warn_if_uninitialized(esp(), "pop32");
    auto value = read_memory32({ ss(), esp().value() });
    set_esp({ esp().value() + sizeof(u32), esp().shadow() });
    return value;
}

void SoftCPU::push16(ValueWithShadow<u16> value)
{
    warn_if_uninitialized(esp(), "push16");
    set_esp({ esp().value() - sizeof(u16), esp().shadow() });
    write_memory16({ ss(), esp().value() }, value);
}

ValueWithShadow<u16> SoftCPU::pop16()
{
    warn_if_uninitialized(esp(), "pop16");
    auto value = read_memory16({ ss(), esp().value() });
    set_esp({ esp().value() + sizeof(u16), esp().shadow() });
    return value;
}

template<bool check_zf, typename Callback>
void SoftCPU::do_once_or_repeat(const X86::Instruction& insn, Callback callback)
{
    if (!insn.has_rep_prefix())
        return callback();

    while (loop_index(insn.a32()).value()) {
        callback();
        decrement_loop_index(insn.a32());
        if constexpr (check_zf) {
            warn_if_flags_tainted("repz/repnz");
            if (insn.rep_prefix() == X86::Prefix::REPZ && !zf())
                break;
            if (insn.rep_prefix() == X86::Prefix::REPNZ && zf())
                break;
        }
    }
}

template<typename T>
ALWAYS_INLINE static T op_inc(SoftCPU& cpu, T data)
{
    typename T::ValueType result;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("incl %%eax\n"
                     : "=a"(result)
                     : "a"(data.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("incw %%ax\n"
                     : "=a"(result)
                     : "a"(data.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("incb %%al\n"
                     : "=a"(result)
                     : "a"(data.value()));
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszap(new_flags);
    cpu.taint_flags_from(data);
    return shadow_wrap_with_taint_from(result, data);
}

template<typename T>
ALWAYS_INLINE static T op_dec(SoftCPU& cpu, T data)
{
    typename T::ValueType result;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("decl %%eax\n"
                     : "=a"(result)
                     : "a"(data.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("decw %%ax\n"
                     : "=a"(result)
                     : "a"(data.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("decb %%al\n"
                     : "=a"(result)
                     : "a"(data.value()));
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszap(new_flags);
    cpu.taint_flags_from(data);
    return shadow_wrap_with_taint_from(result, data);
}

template<typename T>
ALWAYS_INLINE static T op_xor(SoftCPU& cpu, const T& dest, const T& src)
{
    typename T::ValueType result;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("xorl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("xor %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("xorb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszpc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from(result, dest, src);
}

template<typename T>
ALWAYS_INLINE static T op_or(SoftCPU& cpu, const T& dest, const T& src)
{
    typename T::ValueType result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("orl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("or %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("orb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszpc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from(result, dest, src);
}

template<typename T>
ALWAYS_INLINE static T op_sub(SoftCPU& cpu, const T& dest, const T& src)
{
    typename T::ValueType result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("subl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("subw %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("subb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from(result, dest, src);
}

template<typename T, bool cf>
ALWAYS_INLINE static T op_sbb_impl(SoftCPU& cpu, const T& dest, const T& src)
{
    typename T::ValueType result = 0;
    u32 new_flags = 0;

    if constexpr (cf)
        asm volatile("stc");
    else
        asm volatile("clc");

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("sbbl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("sbbw %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("sbbb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, dest, src);
}

template<typename T>
ALWAYS_INLINE static T op_sbb(SoftCPU& cpu, T& dest, const T& src)
{
    cpu.warn_if_flags_tainted("sbb");
    if (cpu.cf())
        return op_sbb_impl<T, true>(cpu, dest, src);
    return op_sbb_impl<T, false>(cpu, dest, src);
}

template<typename T>
ALWAYS_INLINE static T op_add(SoftCPU& cpu, T& dest, const T& src)
{
    typename T::ValueType result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("addl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("addw %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("addb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, dest, src);
}

template<typename T, bool cf>
ALWAYS_INLINE static T op_adc_impl(SoftCPU& cpu, T& dest, const T& src)
{
    typename T::ValueType result = 0;
    u32 new_flags = 0;

    if constexpr (cf)
        asm volatile("stc");
    else
        asm volatile("clc");

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("adcl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("adcw %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("adcb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, dest, src);
}

template<typename T>
ALWAYS_INLINE static T op_adc(SoftCPU& cpu, T& dest, const T& src)
{
    cpu.warn_if_flags_tainted("adc");
    if (cpu.cf())
        return op_adc_impl<T, true>(cpu, dest, src);
    return op_adc_impl<T, false>(cpu, dest, src);
}

template<typename T>
ALWAYS_INLINE static T op_and(SoftCPU& cpu, const T& dest, const T& src)
{
    typename T::ValueType result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("andl %%ecx, %%eax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("andw %%cx, %%ax\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("andb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(dest.value()), "c"(src.value()));
    } else {
        VERIFY_NOT_REACHED();
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszpc(new_flags);
    cpu.taint_flags_from(dest, src);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, dest, src);
}

template<typename T>
ALWAYS_INLINE static void op_imul(SoftCPU& cpu, const T& dest, const T& src, T& result_high, T& result_low)
{
    bool did_overflow = false;
    if constexpr (sizeof(T) == 4) {
        i64 result = (i64)src * (i64)dest;
        result_low = result & 0xffffffff;
        result_high = result >> 32;
        did_overflow = (result > NumericLimits<T>::max() || result < NumericLimits<T>::min());
    } else if constexpr (sizeof(T) == 2) {
        i32 result = (i32)src * (i32)dest;
        result_low = result & 0xffff;
        result_high = result >> 16;
        did_overflow = (result > NumericLimits<T>::max() || result < NumericLimits<T>::min());
    } else if constexpr (sizeof(T) == 1) {
        i16 result = (i16)src * (i16)dest;
        result_low = result & 0xff;
        result_high = result >> 8;
        did_overflow = (result > NumericLimits<T>::max() || result < NumericLimits<T>::min());
    }

    if (did_overflow) {
        cpu.set_cf(true);
        cpu.set_of(true);
    } else {
        cpu.set_cf(false);
        cpu.set_of(false);
    }
}

template<typename T>
ALWAYS_INLINE static T op_shr(SoftCPU& cpu, T data, ValueWithShadow<u8> steps)
{
    if (steps.value() == 0)
        return shadow_wrap_with_taint_from(data.value(), data, steps);

    u32 result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("shrl %%cl, %%eax\n"
                     : "=a"(result)
                     : "a"(data.value()), "c"(steps.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("shrw %%cl, %%ax\n"
                     : "=a"(result)
                     : "a"(data.value()), "c"(steps.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("shrb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(data.value()), "c"(steps.value()));
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(data, steps);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, data, steps);
}

template<typename T>
ALWAYS_INLINE static T op_shl(SoftCPU& cpu, T data, ValueWithShadow<u8> steps)
{
    if (steps.value() == 0)
        return shadow_wrap_with_taint_from(data.value(), data, steps);

    u32 result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("shll %%cl, %%eax\n"
                     : "=a"(result)
                     : "a"(data.value()), "c"(steps.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("shlw %%cl, %%ax\n"
                     : "=a"(result)
                     : "a"(data.value()), "c"(steps.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 1) {
        asm volatile("shlb %%cl, %%al\n"
                     : "=a"(result)
                     : "a"(data.value()), "c"(steps.value()));
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(data, steps);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, data, steps);
}

template<typename T>
ALWAYS_INLINE static T op_shrd(SoftCPU& cpu, T data, T extra_bits, ValueWithShadow<u8> steps)
{
    if (steps.value() == 0)
        return shadow_wrap_with_taint_from(data.value(), data, steps);

    u32 result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("shrd %%cl, %%edx, %%eax\n"
                     : "=a"(result)
                     : "a"(data.value()), "d"(extra_bits.value()), "c"(steps.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("shrd %%cl, %%dx, %%ax\n"
                     : "=a"(result)
                     : "a"(data.value()), "d"(extra_bits.value()), "c"(steps.value()));
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(data, steps);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, data, steps);
}

template<typename T>
ALWAYS_INLINE static T op_shld(SoftCPU& cpu, T data, T extra_bits, ValueWithShadow<u8> steps)
{
    if (steps.value() == 0)
        return shadow_wrap_with_taint_from(data.value(), data, steps);

    u32 result = 0;
    u32 new_flags = 0;

    if constexpr (sizeof(typename T::ValueType) == 4) {
        asm volatile("shld %%cl, %%edx, %%eax\n"
                     : "=a"(result)
                     : "a"(data.value()), "d"(extra_bits.value()), "c"(steps.value()));
    } else if constexpr (sizeof(typename T::ValueType) == 2) {
        asm volatile("shld %%cl, %%dx, %%ax\n"
                     : "=a"(result)
                     : "a"(data.value()), "d"(extra_bits.value()), "c"(steps.value()));
    }

    asm volatile(
        "pushf\n"
        "pop %%ebx"
        : "=b"(new_flags));

    cpu.set_flags_oszapc(new_flags);
    cpu.taint_flags_from(data, steps);
    return shadow_wrap_with_taint_from<typename T::ValueType>(result, data, steps);
}

template<bool update_dest, bool is_or, typename Op>
ALWAYS_INLINE void SoftCPU::generic_AL_imm8(Op op, const X86::Instruction& insn)
{
    auto dest = al();
    auto src = shadow_wrap_as_initialized(insn.imm8());
    auto result = op(*this, dest, src);
    if (is_or && insn.imm8() == 0xff)
        result.set_initialized();
    if (update_dest)
        set_al(result);
}

template<bool update_dest, bool is_or, typename Op>
ALWAYS_INLINE void SoftCPU::generic_AX_imm16(Op op, const X86::Instruction& insn)
{
    auto dest = ax();
    auto src = shadow_wrap_as_initialized(insn.imm16());
    auto result = op(*this, dest, src);
    if (is_or && insn.imm16() == 0xffff)
        result.set_initialized();
    if (update_dest)
        set_ax(result);
}


}
