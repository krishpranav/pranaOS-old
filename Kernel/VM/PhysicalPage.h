#pragma once

// includes
#include <AK/NonnullRefPtr.h>
#include <Kernel/Arch/x86/CPU.h>
#include <Kernel/Assertions.h>
#include <Kernel/Heap/SlabAllocator.h>
#include <Kernel/PhysicalAddress.h>

namespace Kernel {

class PhysicalPage {
    friend class MemoryManager;
    friend class PageDirectory;
    friend class VMObject;

    MAKE_SLAB_ALLOCATED(PhysicalPage);
    AK_MAKE_NONMOVABLE(PhysicalPage);

public:
    PhysicalAddress paddr() const { return m_paddr; }

    void ref()
    {
        m_ref_count.fetch_add(1, AK::memory_order_acq_rel);
    }

    void unref()
    {
        if (m_ref_count.fetch_sub(1, AK::memory_order_acq_rel) == 1) {
            if (m_may_return_to_freelist)
                return_to_freelist();
            delete this;
        }
    }

    static NonnullRefPtr<PhysicalPage> create(PhysicalAddress, bool supervisor, bool may_return_to_freelist = true);

    u32 ref_count() const { return m_ref_count.load(AK::memory_order_consume); }

    bool is_shared_zero_page() const;
    bool is_lazy_committed_page() const;

private:
    PhysicalPage(PhysicalAddress paddr, bool supervisor, bool may_return_to_freelist = true);
    ~PhysicalPage() = default;

    void return_to_freelist() const;

    Atomic<u32> m_ref_count { 1 };
    bool m_may_return_to_freelist { true };
    bool m_supervisor { false };
    PhysicalAddress m_paddr;
};

}
