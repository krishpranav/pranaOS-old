

#pragma once

#include <AKF/Iterator.h>

namespace AKF {

template<typename Container, typename ValueType>
constexpr bool any_of(
    const SimpleIterator<Container, ValueType>& begin,
    const SimpleIterator<Container, ValueType>& end,
    const auto& predicate)
{
    for (auto iter = begin; iter != end; ++iter) {
        if (predicate(*iter)) {
            return true;
        }
    }
    return false;
}

}

using AKF::any_of;
