

#pragma once

#include <AKF/Traits.h>
#include <AKF/Types.h>

namespace AKF {

template<typename TIterator, typename TUnaryPredicate>
constexpr TIterator find_if(TIterator first, TIterator last, TUnaryPredicate&& pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            return first;
        }
    }
    return last;
}

template<typename TIterator, typename T>
constexpr TIterator find(TIterator first, TIterator last, const T& value)
{
    return find_if(first, last, [&](const auto& v) { return Traits<T>::equals(value, v); });
}

template<typename TIterator, typename T>
constexpr size_t find_index(TIterator first, TIterator last, const T& value)
{
    return find_if(first, last, [&](const auto& v) { return Traits<T>::equals(value, v); }).index();
}

}
