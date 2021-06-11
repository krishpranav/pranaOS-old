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
    : m_executable_path(executable_path)
    , m_arguments(arguments)
    , m_environment(environment)
    , m_mmu(*this)
    , m_cpu(*this)
{
    m_malloc_tracer = make<MallocTracer>(*this);

    static constexpr FlatPtr userspace_range_base = 0x00800000;
    static constexpr FlatPtr userspace_range_ceiling = 0xbe000000;
#ifdef UE_ASLR
    static constexpr FlatPtr page_mask = 0xfffff000u;
    size_t random_offset = (get_random<u8>() % 32 * MiB) & page_mask;
    FlatPtr base = userspace_range_base + random_offset;
#else
    FlatPtr base = userspace_range_base;
#endif

    m_range_allocator.initialize_with_range(VirtualAddress(base), userspace_range_ceiling - base);

    VERIFY(!s_the);
    s_the = this;
    register_signal_handlers();
    setup_signal_trampoline();
}

Vector<ELF::AuxiliaryValue> Emulator::generate_auxiliary_vector(FlatPtr load_base, FlatPtr entry_eip, String executable_path, int executable_fd) const
{
    Vector<ELF::AuxiliaryValue> auxv;
    auxv.append({ ELF::AuxiliaryValue::PageSize, PAGE_SIZE });
    auxv.append({ ELF::AuxiliaryValue::BaseAddress, (void*)load_base });

    auxv.append({ ELF::AuxiliaryValue::Entry, (void*)entry_eip });

    auxv.append({ ELF::AuxiliaryValue::Platform, "i386" });

    auxv.append({ ELF::AuxiliaryValue::ExecFilename, executable_path });

    auxv.append({ ELF::AuxiliaryValue::ExecFileDescriptor, executable_fd });

    auxv.append({ ELF::AuxiliaryValue::Null, 0L });
    return auxv;
}

void Emulator::setup_stack(Vector<ELF::AuxiliaryValue> aux_vector)
{
    auto stack_region = make<SimpleRegion>(stack_location, stack_size);
    stack_region->set_stack(true);
    m_mmu.add_region(move(stack_region));
    m_cpu.set_esp(shadow_wrap_as_initialized<u32>(stack_location + stack_size));

    Vector<u32> argv_entries;

    for (auto& argument : m_arguments) {
        m_cpu.push_string(argument.characters());
        argv_entries.append(m_cpu.esp().value());
    }

    Vector<u32> env_entries;

    for (auto& variable : m_environment) {
        m_cpu.push_string(variable.characters());
        env_entries.append(m_cpu.esp().value());
    }

    for (auto& auxv : aux_vector) {
        if (!auxv.optional_string.is_empty()) {
            m_cpu.push_string(auxv.optional_string.characters());
            auxv.auxv.a_un.a_ptr = (void*)m_cpu.esp().value();
        }
    }

    for (ssize_t i = aux_vector.size() - 1; i >= 0; --i) {
        auto& value = aux_vector[i].auxv;
        m_cpu.push_buffer((const u8*)&value, sizeof(value));
    }

    m_cpu.push32(shadow_wrap_as_initialized<u32>(0)); // char** envp = { envv_entries..., nullptr }
    for (ssize_t i = env_entries.size() - 1; i >= 0; --i)
        m_cpu.push32(shadow_wrap_as_initialized(env_entries[i]));
    u32 envp = m_cpu.esp().value();

    m_cpu.push32(shadow_wrap_as_initialized<u32>(0)); // char** argv = { argv_entries..., nullptr }
    for (ssize_t i = argv_entries.size() - 1; i >= 0; --i)
        m_cpu.push32(shadow_wrap_as_initialized(argv_entries[i]));
    u32 argv = m_cpu.esp().value();

    m_cpu.push32(shadow_wrap_as_initialized<u32>(0)); // (alignment)

    u32 argc = argv_entries.size();
    m_cpu.push32(shadow_wrap_as_initialized(envp));
    m_cpu.push32(shadow_wrap_as_initialized(argv));
    m_cpu.push32(shadow_wrap_as_initialized(argc));
    m_cpu.push32(shadow_wrap_as_initialized<u32>(0)); // (alignment)
}


}
