

#pragma once

#include <AKF/Assertions.h>
#include <AKF/Atomic.h>
#include <AKF/Noncopyable.h>
#ifdef KERNEL
#    include <Kernel/Arch/x86/CPU.h>
#endif

#ifndef __pranaos__
#    include <new>
#endif

namespace AKF {

template<typename T>
struct SingletonInstanceCreator {
    static T* create()
    {
        return new T();
    }
};

template<typename T, T* (*InitFunction)() = SingletonInstanceCreator<T>::create>
class Singleton {
    AKF_MAKFE_NONCOPYABLE(Singleton);
    AKF_MAKFE_NONMOVABLE(Singleton);

public:
    Singleton() = default;

    template<bool allow_create = true>
    static T* get(T*& obj_var)
    {
        T* obj = AKF::atomic_load(&obj_var, AKF::memory_order_acquire);
        if (FlatPtr(obj) <= 0x1) {
            // If this is the first time, see if we get to initialize it
#ifdef KERNEL
            Kernel::ScopedCritical critical;
#endif
            if constexpr (allow_create) {
                if (obj == nullptr && AKF::atomic_compare_exchange_strong(&obj_var, obj, (T*)0x1, AKF::memory_order_acq_rel)) {
                    // We're the first one
                    obj = InitFunction();
                    AKF::atomic_store(&obj_var, obj, AKF::memory_order_release);
                    return obj;
                }
            }
            // Someone else was faster, wait until they're done
            while (obj == (T*)0x1) {
#ifdef KERNEL
                Kernel::Processor::wait_check();
#else
                // TODO: yield
#endif
                obj = AKF::atomic_load(&obj_var, AKF::memory_order_acquire);
            }
            if constexpr (allow_create) {
                // We should always return an instance if we allow creating one
                VERIFY(obj != nullptr);
            }
            VERIFY(obj != (T*)0x1);
        }
        return obj;
    }

    T* ptr() const
    {
        return get(m_obj);
    }

    T* operator->() const
    {
        return ptr();
    }

    T& operator*() const
    {
        return *ptr();
    }

    operator T*() const
    {
        return ptr();
    }

    operator T&() const
    {
        return *ptr();
    }

    bool is_initialized() const
    {
        T* obj = AKF::atomic_load(&m_obj, AKF::memory_order_consume);
        return FlatPtr(obj) > 0x1;
    }

    void ensure_instance()
    {
        ptr();
    }

private:
    mutable T* m_obj { nullptr }; // atomic
};

}
