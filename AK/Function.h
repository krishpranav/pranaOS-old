#pragma once

#include <AK/Assertions.h>
#include <AK/OwnPtr.h>
#include <AK/StdLibExtras.h>

namespace AK {

template<typename>
class Function;

template<typename Out, typename... In>
class Function<Out(In...)> {
public:
    Function() = default;

    template<typename CallableType, class = typename EnableIf<!(IsPointer<CallableType> && IsFunction<RemovePointer<CallableType>>)&&IsRvalueReference<CallableType&&>>::Type>
    Function(CallableType&& callable)
        : m_callable_wrapper(make<CallableWrapper<CallableType>>(move(callable)))
    {
    }

    template<typename FunctionType, class = typename EnableIf<IsPointer<FunctionType> && IsFunction<RemovePointer<FunctionType>>>::Type>
    Function(FunctionType f)
        : m_callable_wrapper(make<CallableWrapper<FunctionType>>(move(f)))
    {
    }

    Out operator()(In... in) const
    {
        VERIFY(m_callable_wrapper);
        return m_callable_wrapper->call(forward<In>(in)...);
    }

    explicit operator bool() const { return !!m_callable_wrapper; }

    template<typename CallableType, class = typename EnableIf<!(IsPointer<CallableType> && IsFunction<RemovePointer<CallableType>>)&&IsRvalueReference<CallableType&&>>::Type>
    Function& operator=(CallableType&& callable)
    {
        m_callable_wrapper = make<CallableWrapper<CallableType>>(move(callable));
        return *this;
    }

    template<typename FunctionType, class = typename EnableIf<IsPointer<FunctionType> && IsFunction<RemovePointer<FunctionType>>>::Type>
    Function& operator=(FunctionType f)
    {
        m_callable_wrapper = make<CallableWrapper<FunctionType>>(move(f));
        return *this;
    }

    Function& operator=(std::nullptr_t)
    {
        m_callable_wrapper = nullptr;
        return *this;
    }

private:
    class CallableWrapperBase {
    public:
        virtual ~CallableWrapperBase() = default;
        virtual Out call(In...) const = 0;
    };

    template<typename CallableType>
    class CallableWrapper final : public CallableWrapperBase {
    public:
        explicit CallableWrapper(CallableType&& callable)
            : m_callable(move(callable))
        {
        }

        CallableWrapper(const CallableWrapper&) = delete;
        CallableWrapper& operator=(const CallableWrapper&) = delete;

        Out call(In... in) const final override
        {
            if constexpr (requires { m_callable(forward<In>(in)...); }) {
                return m_callable(forward<In>(in)...);
            } else if constexpr (requires { m_callable(); }) {
                return m_callable();
            } else if constexpr (IsVoid<Out>) {
                return;
            } else {
                return {};
            }
        }

    private:
        CallableType m_callable;
    };

    OwnPtr<CallableWrapperBase> m_callable_wrapper;
};

}

using AK::Function;
