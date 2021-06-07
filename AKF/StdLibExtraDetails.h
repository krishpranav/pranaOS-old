

#pragma once

namespace AKF::Detail {

template<bool B, class T = void>
struct EnableIf {
};

template<class T>
struct EnableIf<true, T> {
    using Type = T;
};

template<class T, T v>
struct IntegralConstant {
    static constexpr T value = v;
    using ValueType = T;
    using Type = IntegralConstant;
    constexpr operator ValueType() const { return value; }
    constexpr ValueType operator()() const { return value; }
};

using FalseType = IntegralConstant<bool, false>;
using TrueType = IntegralConstant<bool, true>;

template<class T>
using AddConst = const T;

template<class T>
struct __RemoveConst {
    using Type = T;
};
template<class T>
struct __RemoveConst<const T> {
    using Type = T;
};
template<class T>
using RemoveConst = typename __RemoveConst<T>::Type;

template<class T>
struct __RemoveVolatile {
    using Type = T;
};

template<class T>
struct __RemoveVolatile<volatile T> {
    using Type = T;
};

template<typename T>
using RemoveVolatile = typename __RemoveVolatile<T>::Type;

template<class T>
using RemoveCV = RemoveVolatile<RemoveConst<T>>;

template<typename...>
using VoidType = void;

template<class T>
inline constexpr bool IsLvalueReference = false;

template<class T>
inline constexpr bool IsLvalueReference<T&> = true;

template<class T>
inline constexpr bool __IsPointerHelper = false;

template<class T>
inline constexpr bool __IsPointerHelper<T*> = true;

template<class T>
inline constexpr bool IsPointer = __IsPointerHelper<RemoveCV<T>>;

template<class>
inline constexpr bool IsFunction = false;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...)> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...)> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) const> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) volatile> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) volatile> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) const volatile> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const volatile> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...)&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...)&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) const&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) volatile&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) volatile&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) const volatile&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const volatile&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) &&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) &&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) const&&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const&&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) volatile&&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) volatile&&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args...) const volatile&&> = true;
template<class Ret, class... Args>
inline constexpr bool IsFunction<Ret(Args..., ...) const volatile&&> = true;

template<class T>
inline constexpr bool IsRvalueReference = false;
template<class T>
inline constexpr bool IsRvalueReference<T&&> = true;

template<class T>
struct __RemovePointer {
    using Type = T;
};
template<class T>
struct __RemovePointer<T*> {
    using Type = T;
};
template<class T>
struct __RemovePointer<T* const> {
    using Type = T;
};
template<class T>
struct __RemovePointer<T* volatile> {
    using Type = T;
};
template<class T>
struct __RemovePointer<T* const volatile> {
    using Type = T;
};
template<typename T>
using RemovePointer = typename __RemovePointer<T>::Type;

template<typename T, typename U>
inline constexpr bool IsSame = false;

template<typename T>
inline constexpr bool IsSame<T, T> = true;

template<bool condition, class TrueType, class FalseType>
struct __Conditional {
    using Type = TrueType;
};

template<class TrueType, class FalseType>
struct __Conditional<false, TrueType, FalseType> {
    using Type = FalseType;
};

template<bool condition, class TrueType, class FalseType>
using Conditional = typename __Conditional<condition, TrueType, FalseType>::Type;

template<typename T>
inline constexpr bool IsNullPointer = IsSame<decltype(nullptr), RemoveCV<T>>;

template<typename T>
struct __RemoveReference {
    using Type = T;
};
template<class T>
struct __RemoveReference<T&> {
    using Type = T;
};
template<class T>
struct __RemoveReference<T&&> {
    using Type = T;
};

template<typename T>
using RemoveReference = typename __RemoveReference<T>::Type;

template<typename T>
struct __MAKFeUnsigned {
    using Type = void;
};
template<>
struct __MAKFeUnsigned<signed char> {
    using Type = unsigned char;
};
template<>
struct __MAKFeUnsigned<short> {
    using Type = unsigned short;
};
template<>
struct __MAKFeUnsigned<int> {
    using Type = unsigned int;
};
template<>
struct __MAKFeUnsigned<long> {
    using Type = unsigned long;
};
template<>
struct __MAKFeUnsigned<long long> {
    using Type = unsigned long long;
};
template<>
struct __MAKFeUnsigned<unsigned char> {
    using Type = unsigned char;
};
template<>
struct __MAKFeUnsigned<unsigned short> {
    using Type = unsigned short;
};
template<>
struct __MAKFeUnsigned<unsigned int> {
    using Type = unsigned int;
};
template<>
struct __MAKFeUnsigned<unsigned long> {
    using Type = unsigned long;
};
template<>
struct __MAKFeUnsigned<unsigned long long> {
    using Type = unsigned long long;
};
template<>
struct __MAKFeUnsigned<char> {
    using Type = unsigned char;
};
template<>
struct __MAKFeUnsigned<char8_t> {
    using Type = char8_t;
};
template<>
struct __MAKFeUnsigned<char16_t> {
    using Type = char16_t;
};
template<>
struct __MAKFeUnsigned<char32_t> {
    using Type = char32_t;
};
template<>
struct __MAKFeUnsigned<bool> {
    using Type = bool;
};

template<typename T>
using MAKFeUnsigned = typename __MAKFeUnsigned<T>::Type;

template<typename T>
struct __MAKFeSigned {
};
template<>
struct __MAKFeSigned<signed char> {
    using Type = signed char;
};
template<>
struct __MAKFeSigned<short> {
    using Type = short;
};
template<>
struct __MAKFeSigned<int> {
    using Type = int;
};
template<>
struct __MAKFeSigned<long> {
    using Type = long;
};
template<>
struct __MAKFeSigned<long long> {
    using Type = long long;
};
template<>
struct __MAKFeSigned<unsigned char> {
    using Type = char;
};
template<>
struct __MAKFeSigned<unsigned short> {
    using Type = short;
};
template<>
struct __MAKFeSigned<unsigned int> {
    using Type = int;
};
template<>
struct __MAKFeSigned<unsigned long> {
    using Type = long;
};
template<>
struct __MAKFeSigned<unsigned long long> {
    using Type = long long;
};
template<>
struct __MAKFeSigned<char> {
    using Type = char;
};

template<typename T>
using MAKFeSigned = typename __MAKFeSigned<T>::Type;

template<class T>
inline constexpr bool IsVoid = IsSame<void, RemoveCV<T>>;

template<class T>
inline constexpr bool IsConst = false;

template<class T>
inline constexpr bool IsConst<const T> = true;

template<typename T>
inline constexpr bool IsEnum = __is_enum(T);

template<typename T>
inline constexpr bool IsUnion = __is_union(T);

template<typename T>
inline constexpr bool IsClass = __is_class(T);

template<typename Base, typename Derived>
inline constexpr bool IsBaseOf = __is_base_of(Base, Derived);

template<typename T>
inline constexpr bool __IsIntegral = false;

template<>
inline constexpr bool __IsIntegral<bool> = true;
template<>
inline constexpr bool __IsIntegral<unsigned char> = true;
template<>
inline constexpr bool __IsIntegral<char8_t> = true;
template<>
inline constexpr bool __IsIntegral<char16_t> = true;
template<>
inline constexpr bool __IsIntegral<char32_t> = true;
template<>
inline constexpr bool __IsIntegral<unsigned short> = true;
template<>
inline constexpr bool __IsIntegral<unsigned int> = true;
template<>
inline constexpr bool __IsIntegral<unsigned long> = true;
template<>
inline constexpr bool __IsIntegral<unsigned long long> = true;

template<typename T>
inline constexpr bool IsIntegral = __IsIntegral<MAKFeUnsigned<RemoveCV<T>>>;

template<typename T>
inline constexpr bool __IsFloatingPoint = false;
template<>
inline constexpr bool __IsFloatingPoint<float> = true;
template<>
inline constexpr bool __IsFloatingPoint<double> = true;
template<>
inline constexpr bool __IsFloatingPoint<long double> = true;

template<typename T>
inline constexpr bool IsFloatingPoint = __IsFloatingPoint<RemoveCV<T>>;

template<typename ReferenceType, typename T>
using CopyConst = Conditional<IsConst<ReferenceType>, AddConst<T>, RemoveConst<T>>;

template<typename... Ts>
using Void = void;

template<typename... _Ignored>
constexpr auto DependentFalse = false;

template<typename T>
inline constexpr bool IsSigned = IsSame<T, MAKFeSigned<T>>;

template<typename T>
inline constexpr bool IsUnsigned = IsSame<T, MAKFeUnsigned<T>>;

template<typename T>
inline constexpr bool IsArithmetic = IsIntegral<T> || IsFloatingPoint<T>;

template<typename T>
inline constexpr bool IsFundamental = IsArithmetic<T> || IsVoid<T> || IsNullPointer<T>;

template<typename T, T... Ts>
struct IntegerSequence {
    using Type = T;
    static constexpr unsigned size() noexcept { return sizeof...(Ts); };
};

template<unsigned... Indices>
using IndexSequence = IntegerSequence<unsigned, Indices...>;

template<typename T, T N, T... Ts>
auto mAKFe_integer_sequence_impl()
{
    if constexpr (N == 0)
        return IntegerSequence<T, Ts...> {};
    else
        return mAKFe_integer_sequence_impl<T, N - 1, N - 1, Ts...>();
}

template<typename T, T N>
using MAKFeIntegerSequence = decltype(mAKFe_integer_sequence_impl<T, N>());

template<unsigned N>
using MAKFeIndexSequence = MAKFeIntegerSequence<unsigned, N>;

template<typename T>
struct __IdentityType {
    using Type = T;
};

template<typename T>
using IdentityType = typename __IdentityType<T>::Type;

template<class T>
requires(IsEnum<T>) using UnderlyingType = __underlying_type(T);

template<typename T>
inline constexpr bool IsTrivial = __is_trivial(T);

template<typename T>
inline constexpr bool IsTriviallyCopyable = __is_trivially_copyable(T);

}
using AKF::Detail::AddConst;
using AKF::Detail::Conditional;
using AKF::Detail::CopyConst;
using AKF::Detail::DependentFalse;
using AKF::Detail::EnableIf;
using AKF::Detail::FalseType;
using AKF::Detail::IdentityType;
using AKF::Detail::IndexSequence;
using AKF::Detail::IntegerSequence;
using AKF::Detail::IsArithmetic;
using AKF::Detail::IsBaseOf;
using AKF::Detail::IsClass;
using AKF::Detail::IsConst;
using AKF::Detail::IsEnum;
using AKF::Detail::IsFloatingPoint;
using AKF::Detail::IsFunction;
using AKF::Detail::IsFundamental;
using AKF::Detail::IsIntegral;
using AKF::Detail::IsLvalueReference;
using AKF::Detail::IsNullPointer;
using AKF::Detail::IsPointer;
using AKF::Detail::IsRvalueReference;
using AKF::Detail::IsSame;
using AKF::Detail::IsSigned;
using AKF::Detail::IsTrivial;
using AKF::Detail::IsTriviallyCopyable;
using AKF::Detail::IsUnion;
using AKF::Detail::IsUnsigned;
using AKF::Detail::IsVoid;
using AKF::Detail::MAKFeIndexSequence;
using AKF::Detail::MAKFeIntegerSequence;
using AKF::Detail::MAKFeSigned;
using AKF::Detail::MAKFeUnsigned;
using AKF::Detail::RemoveConst;
using AKF::Detail::RemoveCV;
using AKF::Detail::RemovePointer;
using AKF::Detail::RemoveReference;
using AKF::Detail::RemoveVolatile;
using AKF::Detail::TrueType;
using AKF::Detail::UnderlyingType;
using AKF::Detail::Void;
