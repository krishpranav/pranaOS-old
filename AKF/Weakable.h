
#pragma once

#include "Assertions.h"
#include "Atomic.h"
#include "RefCounted.h"
#include "RefPtr.h"
#include "StdLibExtras.h"
#ifdef KERNEL
#    include <Kernel/Arch/x86/CPU.h>
#endif

namespace AKF {

template<typename T>
class WeAKFable;
template<typename T>
class WeAKFPtr;

class WeAKFLink : public RefCounted<WeAKFLink> {
    template<typename T>
    friend class WeAKFable;
    template<typename T>
    friend class WeAKFPtr;

public:
    template<typename T, typename PtrTraits = RefPtrTraits<T>, typename EnableIf<IsBaseOf<RefCountedBase, T>>::Type* = nullptr>
    RefPtr<T, PtrTraits> strong_ref() const
    {
        RefPtr<T, PtrTraits> ref;

        {
#ifdef KERNEL
            // We don't want to be pre-empted while we are trying to obtain
            // a strong reference
            Kernel::ScopedCritical critical;
#endif
            if (!(m_consumers.fetch_add(1u << 1, AKF::MemoryOrder::memory_order_acquire) & 1u)) {
                T* ptr = (T*)m_ptr.load(AKF::MemoryOrder::memory_order_acquire);
                if (ptr && ptr->try_ref())
                    ref = adopt_ref(*ptr);
            }
            m_consumers.fetch_sub(1u << 1, AKF::MemoryOrder::memory_order_release);
        }

        return ref;
    }

    template<typename T>
    T* unsafe_ptr() const
    {
        if (m_consumers.load(AKF::MemoryOrder::memory_order_relaxed) & 1u)
            return nullptr;
        // NOTE: This may return a non-null pointer even if revocation
        // has been triggered as there is a possible race! But it's "unsafe"
        // anyway because we return a raw pointer without ensuring a
        // reference...
        return (T*)m_ptr.load(AKF::MemoryOrder::memory_order_acquire);
    }

    bool is_null() const
    {
        return !unsafe_ptr<void>();
    }

    void revoke()
    {
        auto current_consumers = m_consumers.fetch_or(1u, AKF::MemoryOrder::memory_order_relaxed);
        VERIFY(!(current_consumers & 1u));
        // We flagged revokation, now wait until everyone trying to obtain
        // a strong reference is done
        while (current_consumers > 0) {
#ifdef KERNEL
            Kernel::Processor::wait_check();
#else
            // TODO: yield?
#endif
            current_consumers = m_consumers.load(AKF::MemoryOrder::memory_order_acquire) & ~1u;
        }
        // No one is trying to use it (anymore)
        m_ptr.store(nullptr, AKF::MemoryOrder::memory_order_release);
    }

private:
    template<typename T>
    explicit WeAKFLink(T& weAKFable)
        : m_ptr(&weAKFable)
    {
    }
    mutable Atomic<void*> m_ptr;
    mutable Atomic<unsigned> m_consumers; // LSB indicates revokation in progress
};

template<typename T>
class WeAKFable {
private:
    class Link;

public:
    template<typename U = T>
    WeAKFPtr<U> mAKFe_weAKF_ptr() const;

protected:
    WeAKFable() = default;

    ~WeAKFable()
    {
        m_being_destroyed.store(true, AKF::MemoryOrder::memory_order_release);
        revoke_weAKF_ptrs();
    }

    void revoke_weAKF_ptrs()
    {
        if (auto link = move(m_link))
            link->revoke();
    }

private:
    mutable RefPtr<WeAKFLink> m_link;
    Atomic<bool> m_being_destroyed { false };
};

}

using AKF::WeAKFable;
