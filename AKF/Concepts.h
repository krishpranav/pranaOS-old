
#pragma once

#include <AKF/StdLibExtras.h>

namespace AKF::Concepts {

#if defined(__cpp_concepts) && !defined(__COVERITY__)

template<typename T>
concept Integral = IsIntegral<T>;

template<typename T>
concept FloatingPoint = IsFloatingPoint<T>;

template<typename T>
concept Arithmetic = IsArithmetic<T>;

template<typename T>
concept Signed = IsSigned<T>;

template<typename T>
concept Unsigned = IsUnsigned<T>;

#endif

}

#if defined(__cpp_concepts) && !defined(__COVERITY__)

using AKF::Concepts::Arithmetic;
using AKF::Concepts::FloatingPoint;
using AKF::Concepts::Integral;
using AKF::Concepts::Signed;
using AKF::Concepts::Unsigned;

#endif
