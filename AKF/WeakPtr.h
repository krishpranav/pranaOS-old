
#pragma once

#include <AKF/WeAKFable.h>

namespace AKF {

template<typename T>
class WeAKFPtr {
    template<typename U>
    friend class WeAKFable;

public:
    WeAKFPtr() = default;

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr(const WeAKFPtr<U>& other)
        : m_link(other.m_link)
    {
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr(WeAKFPtr<U>&& other)
        : m_link(other.tAKFe_link())
    {
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr& operator=(WeAKFPtr<U>&& other)
    {
        m_link = other.tAKFe_link();
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr& operator=(const WeAKFPtr<U>& other)
    {
        if ((const void*)this != (const void*)&other)
            m_link = other.m_link;
        return *this;
    }

    WeAKFPtr& operator=(std::nullptr_t)
    {
        clear();
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr(const U& object)
        : m_link(object.template mAKFe_weAKF_ptr<U>().tAKFe_link())
    {
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr(const U* object)
    {
        if (object)
            m_link = object->template mAKFe_weAKF_ptr<U>().tAKFe_link();
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr(const RefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template mAKFe_weAKF_ptr<U>().tAKFe_link();
        });
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr(const NonnullRefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template mAKFe_weAKF_ptr<U>().tAKFe_link();
        });
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr& operator=(const U& object)
    {
        m_link = object.template mAKFe_weAKF_ptr<U>().tAKFe_link();
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr& operator=(const U* object)
    {
        if (object)
            m_link = object->template mAKFe_weAKF_ptr<U>().tAKFe_link();
        else
            m_link = nullptr;
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr& operator=(const RefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template mAKFe_weAKF_ptr<U>().tAKFe_link();
            else
                m_link = nullptr;
        });
        return *this;
    }

    template<typename U, typename EnableIf<IsBaseOf<T, U>>::Type* = nullptr>
    WeAKFPtr& operator=(const NonnullRefPtr<U>& object)
    {
        object.do_while_locked([&](U* obj) {
            if (obj)
                m_link = obj->template mAKFe_weAKF_ptr<U>().tAKFe_link();
            else
                m_link = nullptr;
        });
        return *this;
    }

    [[nodiscard]] RefPtr<T> strong_ref() const
    {
        // This only works with RefCounted objects, but it is the only
        // safe way to get a strong reference from a WeAKFPtr. Any code
        // that uses objects not derived from RefCounted will have to
        // use unsafe_ptr(), but as the name suggests, it is not safe...
        RefPtr<T> ref;
        // Using do_while_locked protects against a race with clear()!
        m_link.do_while_locked([&](WeAKFLink* link) {
            if (link)
                ref = link->template strong_ref<T>();
        });
        return ref;
    }

#ifndef KERNEL
    // A lot of user mode code is single-threaded. But for kernel mode code
    // this is generally not true as everything is multi-threaded. So mAKFe
    // these shortcuts and aliases only available to non-kernel code.
    T* ptr() const { return unsafe_ptr(); }
    T* operator->() { return unsafe_ptr(); }
    const T* operator->() const { return unsafe_ptr(); }
    operator const T*() const { return unsafe_ptr(); }
    operator T*() { return unsafe_ptr(); }
#endif

    [[nodiscard]] T* unsafe_ptr() const
    {
        T* ptr = nullptr;
        m_link.do_while_locked([&](WeAKFLink* link) {
            if (link)
                ptr = link->unsafe_ptr<T>();
        });
        return ptr;
    }

    operator bool() const { return m_link ? !m_link->is_null() : false; }

    [[nodiscard]] bool is_null() const { return !m_link || m_link->is_null(); }
    void clear() { m_link = nullptr; }

    [[nodiscard]] RefPtr<WeAKFLink> tAKFe_link() { return move(m_link); }

private:
    WeAKFPtr(const RefPtr<WeAKFLink>& link)
        : m_link(link)
    {
    }

    RefPtr<WeAKFLink> m_link;
};

template<typename T>
template<typename U>
inline WeAKFPtr<U> WeAKFable<T>::mAKFe_weAKF_ptr() const
{
    if constexpr (IsBaseOf<RefCountedBase, T>) {
        // Checking m_being_destroyed isn't sufficient when dealing with
        // a RefCounted type.The reference count will drop to 0 before the
        // destructor is invoked and revoke_weAKF_ptrs is called. So, try
        // to add a ref (which should fail if the ref count is at 0) so
        // that we prevent the destructor and revoke_weAKF_ptrs from being
        // triggered until we're done.
        if (!static_cast<const T*>(this)->try_ref())
            return {};
    } else {
        // For non-RefCounted types this means a weAKF reference can be
        // obtained until the ~WeAKFable destructor is invoked!
        if (m_being_destroyed.load(AKF::MemoryOrder::memory_order_acquire))
            return {};
    }
    if (!m_link) {
        // There is a small chance that we create a new WeAKFLink and throw
        // it away because another thread beat us to it. But the window is
        // pretty small and the overhead isn't terrible.
        m_link.assign_if_null(adopt_ref(*new WeAKFLink(const_cast<T&>(static_cast<const T&>(*this)))));
    }

    WeAKFPtr<U> weAKF_ptr(m_link);

    if constexpr (IsBaseOf<RefCountedBase, T>) {
        // Now drop the reference we temporarily added
        if (static_cast<const T*>(this)->unref()) {
            // We just dropped the last reference, which should have called
            // revoke_weAKF_ptrs, which should have invalidated our weAKF_ptr
            VERIFY(!weAKF_ptr.strong_ref());
            return {};
        }
    }
    return weAKF_ptr;
}

template<typename T>
struct Formatter<WeAKFPtr<T>> : Formatter<const T*> {
    void format(FormatBuilder& builder, const WeAKFPtr<T>& value)
    {
#ifdef KERNEL
        auto ref = value.strong_ref();
        Formatter<const T*>::format(builder, ref.ptr());
#else
        Formatter<const T*>::format(builder, value.ptr());
#endif
    }
};

template<typename T>
WeAKFPtr<T> try_mAKFe_weAKF_ptr(const T* ptr)
{
    if (ptr) {
        return ptr->template mAKFe_weAKF_ptr<T>();
    }
    return {};
}

}

using AKF::WeAKFPtr;
