
#pragma once

#include <AK/NonnullPtrVector.h>
#include <AK/NonnullRefPtr.h>

namespace AK {

template<typename T, size_t inline_capacity>
class NonnullRefPtrVector : public NonnullPtrVector<NonnullRefPtr<T>, inline_capacity> {
};

}

using AK::NonnullRefPtrVector;
