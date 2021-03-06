// includes
#include <Kernel/Arch/x86/CPU.h>
#include <Kernel/Assertions.h>
#include <Kernel/Debug.h>
#include <Kernel/Interrupts/IRQHandler.h>
#include <Kernel/Interrupts/InterruptManagement.h>
#include <Kernel/Interrupts/PIC.h>
#include <Kernel/Interrupts/SharedIRQHandler.h>

namespace Kernel {

UNMAP_AFTER_INIT void SharedIRQHandler::initialize(u8 interrupt_number)
{
    auto* handler = new SharedIRQHandler(interrupt_number);
    handler->register_interrupt_handler();
    handler->disable_interrupt_vector();
}

void SharedIRQHandler::register_handler(GenericInterruptHandler& handler)
{
    dbgln_if(INTERRUPT_DEBUG, "Interrupt Handler registered @ Shared Interrupt Handler {}", interrupt_number());
    m_handlers.set(&handler);
    enable_interrupt_vector();
}
void SharedIRQHandler::unregister_handler(GenericInterruptHandler& handler)
{
    dbgln_if(INTERRUPT_DEBUG, "Interrupt Handler unregistered @ Shared Interrupt Handler {}", interrupt_number());
    m_handlers.remove(&handler);
    if (m_handlers.is_empty())
        disable_interrupt_vector();
}

bool SharedIRQHandler::eoi()
{
    dbgln_if(INTERRUPT_DEBUG, "EOI IRQ {}", interrupt_number());
    m_responsible_irq_controller->eoi(*this);
    return true;
}

SharedIRQHandler::SharedIRQHandler(u8 irq)
    : GenericInterruptHandler(irq)
    , m_responsible_irq_controller(InterruptManagement::the().get_responsible_irq_controller(irq))
{
    dbgln_if(INTERRUPT_DEBUG, "Shared Interrupt Handler registered @ {}", interrupt_number());
}

SharedIRQHandler::~SharedIRQHandler()
{
    dbgln_if(INTERRUPT_DEBUG, "Shared Interrupt Handler unregistered @ {}", interrupt_number());
    disable_interrupt_vector();
}

void SharedIRQHandler::handle_interrupt(const RegisterState& regs)
{
    VERIFY_INTERRUPTS_DISABLED();

    if constexpr (INTERRUPT_DEBUG) {
        dbgln("Interrupt @ {}", interrupt_number());
        dbgln("Interrupt Handlers registered - {}", m_handlers.size());
    }

    int i = 0;
    for (auto* handler : m_handlers) {
        dbgln_if(INTERRUPT_DEBUG, "Going for Interrupt Handling @ {}, Shared Interrupt {}", i, interrupt_number());
        VERIFY(handler != nullptr);
        handler->increment_invoking_counter();
        handler->handle_interrupt(regs);
        dbgln_if(INTERRUPT_DEBUG, "Going for Interrupt Handling @ {}, Shared Interrupt {} - End", i, interrupt_number());
        i++;
    }
}

void SharedIRQHandler::enable_interrupt_vector()
{
    if (m_enabled)
        return;
    m_enabled = true;
    m_responsible_irq_controller->enable(*this);
}

void SharedIRQHandler::disable_interrupt_vector()
{
    if (!m_enabled)
        return;
    m_enabled = false;
    m_responsible_irq_controller->disable(*this);
}

}
