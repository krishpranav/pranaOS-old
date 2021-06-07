
#pragma once

#include <AKF/Assertions.h>
#include <AKF/Atomic.h>
#include <AKF/Checked.h>
#include <AKF/Noncopyable.h>
#include <AKF/Platform.h>
#include <AKF/StdLibExtras.h>

namespace AKF {

template<class T>
constexpr auto call_will_be_destroyed_if_present(const T* object) -> decltype(const_cast<T*>(object)->will_be_destroyed(), TrueType {})
{
    const_cast<T*>(object)->will_be_destroyed();
    return {};
}

constexpr auto call_will_be_destroyed_if_present(...) -> FalseType
{
    return {};
}

template<class T>
constexpr auto call_one_ref_left_if_present(const T* object) -> decltype(const_cast<T*>(object)->one_ref_left(), TrueType {})
{
    const_cast<T*>(object)->one_ref_left();
    return {};
}

constexpr auto call_one_ref_left_if_present(...) -> FalseType
{
    return {};
}

class RefCountedBase {
    AKF_MAKFE_NONCOPYABLE(RefCountedBase);
    AKF_MAKFE_NONMOVABLE(RefCountedBase);

public:
    using RefCountType = unsigned int;
    using AllowOwnPtr = FalseType;

    ALWAYS_INLINE void ref() const
    {
        auto old_ref_count = m_ref_count.fetch_add(1, AKF::MemoryOrder::memory_order_relaxed);
        VERIFY(old_ref_count > 0);
        VERIFY(!Checked<RefCountType>::addition_would_overflow(old_ref_count, 1));
    }

    [[nodiscard]] ALWAYS_INLINE bool try_ref() const
    {
        RefCountType expected = m_ref_count.load(AKF::MemoryOrder::memory_order_relaxed);
        for (;;) {
            if (expected == 0)
                return false;
            VERIFY(!Checked<RefCountType>::addition_would_overflow(expected, 1));
            if (m_ref_count.compare_exchange_strong(expected, expected + 1, AKF::MemoryOrder::memory_order_acquire))
                return true;
        }
    }

    ALWAYS_INLINE RefCountType ref_count() const
    {
        return m_ref_count.load(AKF::MemoryOrder::memory_order_relaxed);
    }

protected:
    RefCountedBase() = default;
    ALWAYS_INLINE ~RefCountedBase()
    {
        VERIFY(m_ref_count.load(AKF::MemoryOrder::memory_order_relaxed) == 0);
    }

    ALWAYS_INLINE RefCountType deref_base() const
    {
        auto old_ref_count = m_ref_count.fetch_sub(1, AKF::MemoryOrder::memory_order_acq_rel);
        VERIFY(old_ref_count > 0);
        return old_ref_count - 1;
    }

    mutable Atomic<RefCountType> m_ref_count { 1 };
};

template<typename T>
class RefCounted : public RefCountedBase {
public:
    bool unref() const
    {
        auto new_ref_count = deref_base();
        if (new_ref_count == 0) {
            call_will_be_destroyed_if_present(static_cast<const T*>(this));
            delete static_cast<const T*>(this);
            return true;
        } else if (new_ref_count == 1) {
            call_one_ref_left_if_present(static_cast<const T*>(this));
        }
        return false;
    }
};

}

using AKF::RefCounted;
