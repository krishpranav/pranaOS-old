// includes
#include <AK/Memory.h>
#include <AK/kmalloc.h>
#include <Kernel/Arch/x86/CPU.h>
#include <Kernel/ThreadTracer.h>

namespace Kernel {

ThreadTracer::ThreadTracer(ProcessID tracer_pid)
    : m_tracer_pid(tracer_pid)
{
}

void ThreadTracer::set_regs(const RegisterState& regs)
{
    PtraceRegisters r {};
    copy_kernel_registers_into_ptrace_registers(r, regs);
    m_regs = r;
}

}
