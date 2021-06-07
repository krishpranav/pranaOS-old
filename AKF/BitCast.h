

#pragma once

namespace AKF {

template<typename T, typename U>
inline T bit_cast(const U& a)
{
    static_assert(sizeof(T) == sizeof(U));

    T result;
    __builtin_memcpy(&result, &a, sizeof(T));
    return result;
}

}

using AKF::bit_cast;
