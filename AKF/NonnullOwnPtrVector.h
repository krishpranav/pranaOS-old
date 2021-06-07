
#pragma once

#include <AKF/NonnullOwnPtr.h>
#include <AKF/NonnullPtrVector.h>

namespace AKF {

template<typename T, size_t inline_capacity>
class NonnullOwnPtrVector : public NonnullPtrVector<NonnullOwnPtr<T>, inline_capacity> {
};

}

using AKF::NonnullOwnPtrVector;
