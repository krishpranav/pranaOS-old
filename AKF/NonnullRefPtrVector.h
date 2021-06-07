
#pragma once

#include <AKF/NonnullPtrVector.h>
#include <AKF/NonnullRefPtr.h>

namespace AKF {

template<typename T, size_t inline_capacity>
class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> {
};

}

using AKF::NonnullRefPtrVector;
