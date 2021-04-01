#include <libc.h>
#include "idt.h"


struct idt_t {
    unsigned short offset_low;
    unsigned short segment_selector;
    unsigned char reserved;
    unsigned char attr;
    unsigned short offset_high;
} __attribute__((__packed__));

/* This will go into LIDT instruction */
struct idt_ptr {
    unsigned short limit;
    unsigned int addr;
} __attribute__((__packed__));

static struct idt_t _idt[IDT_MAX_INTERRUPTS];

static struct idt_ptr _idt_ptr = {
    .limit = (sizeof(struct idt_t) * IDT_MAX_INTERRUPTS) - 1,
    .addr = (int) _idt
};


/*
 * Registers interrupt handler.
 */
int reg_irq(int irq_line, irq_handler hndl)
{
    if (irq_line < 0 || irq_line > IDT_MAX_INTERRUPTS)
        return -1;
    
    _idt[irq_line].offset_low = ((int) hndl) & 0xFFFF;
    _idt[irq_line].offset_high = (((int) hndl) >> 16) & 0xFFFF;
    _idt[irq_line].segment_selector = 0x8;
    _idt[irq_line].reserved = 0;
    _idt[irq_line].attr = IDT_ATTR_32BIT | IDT_ATTR_DPL_RING_0 | IDT_ATTR_SEGMENT_PRESENT;
    
    return 0;
}

/*
 * Installs IDT.
 */
int install_idt()
{
    __asm__ __volatile__("lidtl %0" : : "m" (_idt_ptr));
    return 0;
}